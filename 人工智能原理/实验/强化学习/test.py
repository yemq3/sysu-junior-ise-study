#!/usr/bin/env python
"""
PyTorch implementation of DQN
Paper: https://www.cs.toronto.edu/~vmnih/docs/dqn.pdf
"""

import argparse
import gym
from gym import wrappers
import numpy as np
import pdb
import os
import random
import time
import gc

import torch
import torch.nn as nn
from torch import optim
import torch.nn.functional as F
import torchvision.transforms as T
from torch.autograd import Variable


EXP_DIR = 'exp'
API_KEY = 'sk_ARsYZ2eRsGoeANVhUgrQ'
ENVS = {
    'breakout': 'Breakout-v0',
    'pong': 'Pong-v0',
}
STATE_PREPFN = {
    'breakout': lambda s: s[50:, :, :],
    'pong': lambda s: s[50:, :, :],
}


def mkdir(base, name):
    path = os.path.join(base, name)
    if not os.path.exists(path):
        os.makedirs(path)
    return path


class ReplayMemory(object):
    """ Facilitates memory replay. """
    def __init__(self, capacity):
        self.capacity = capacity
        self.memory = []
        self.idx = 0

    def push(self, m):
        if len(self.memory) < self.capacity:
            self.memory.append(None)
        self.memory[self.idx] = m
        self.idx = (self.idx + 1) % self.capacity

    def sample(self, bsz):
        batch = random.sample(self.memory, bsz)
        return map(lambda x: Variable(torch.cat(x, 0).cuda()), zip(*batch))


class StatePrep(object):
    """ Preproces the state. """
    def __init__(self, prepfn, size):
        self.prepfn = prepfn
        self.transform = T.Compose([
            T.ToPILImage(),
            T.Lambda(lambda x: x.convert('L')),
            T.Scale(size),
            T.ToTensor()])

    def run(self, s):
        return self.transform(self.prepfn(s)).unsqueeze(0)


class QNet(nn.Module):
    def __init__(self, nA):
        super(QNet, self).__init__()
        self.conv1 = nn.Conv2d(4, 32, kernel_size=8, stride=4)
        self.conv2 = nn.Conv2d(32, 64, kernel_size=4, stride=2)
        self.conv3 = nn.Conv2d(64, 64, kernel_size=3, stride=1)
        self.fc1 = nn.Linear(7 * 7 * 64, 512)
        self.fc2 = nn.Linear(512, nA)

    def forward(self, x):
        x = F.relu(self.conv1(x))
        x = F.relu(self.conv2(x))
        x = F.relu(self.conv3(x))
        x = F.relu(self.fc1(x.view(x.size(0), -1)))
        return self.fc2(x)


def dqn(E, args, work_dir):
    nA = E.action_space.n
    A = np.arange(nA)
    mem = ReplayMemory(args.mem_capacity)
    prep = StatePrep(STATE_PREPFN[args.env], 84)
    Q = QNet(nA).cuda()
    T = QNet(nA).cuda()
    opt = optim.RMSprop(Q.parameters(),
        lr=0.00025, eps=0.001, alpha=0.95)
    crit = nn.MSELoss()

    Q.train()
    T.eval()

    eps = max(args.eps, args.eps_min)
    eps_delta = (eps - args.eps_min) / args.eps_decay_window

    P = np.zeros(nA, np.float32)

    def reset():
        return torch.cat([prep.run(E.reset())] * 4, 1)

    def act(s, eps):
        P.fill(eps / nA)
        q, argq = Q(Variable(s.cuda(), volatile=True)).data.cpu().max(1)
        P[argq.item()] += 1 - eps
        a = np.random.choice(A, p=P)
        ns, r, done, _ = E.step(a)
        ns = torch.cat([s.narrow(1, 1, 3), prep.run(ns)], 1)
        mem.push((s, torch.LongTensor([int(a)]),
            torch.Tensor([r]), ns, torch.Tensor([done])))
        return ns, r, done, q.item()
    print('init replay memory with %d entries' % args.mem_init_size)
    s = reset()
    for _ in range(args.mem_init_size):
        ns, _, done, _ = act(s, eps)
        s = reset() if done else ns

    print('train, max episodes %d' % args.max_episodes)
    t = 0
    R = np.zeros(args.window, np.float32)
    M = np.zeros(args.window, np.float32)
    for e in range(args.max_episodes):
        R[e % args.window] = 0
        M[e % args.window] = -1e9
        s = reset()
        done = False
        ep_len = 0

        if e % args.save_freq == 0:
            model_file = os.path.join(work_dir, 'model_e%d.th' % e)
            with open(model_file, 'wb') as f:
                torch.save(Q, f)
        if e % args.gc_freq == 0:
            gc.collect()

        start_time = time.time()

        while not done:
            if t % args.sync_period == 0:
                T.load_state_dict(Q.state_dict())

            # playout
            cur_eps = max(args.eps_min, eps - eps_delta * t)
            ns, r, done, q = act(s, cur_eps)
            R[e % args.window] += r
            M[e % args.window] = max(M[e % args.window], q)
            s = ns

            t += 1
            ep_len += 1

            # train
            if ep_len % args.learn_freq == 0:
                bs, ba, br, bns, bdone = mem.sample(args.batch_size)
                bq = Q(bs).gather(1, ba.unsqueeze(1)).squeeze(1)
                bnq = T(bns).detach().max(1)[0].squeeze(1) * args.gamma * (1 - bdone)
                loss = crit(bq, br + bnq)
                opt.zero_grad()

                loss.backward()
                #nn.utils.clip_grad_norm(Q.parameters(), args.clip)
                opt.step()

        print('episode %d (%d): time %.2fs mem %d eps %.5f len %d r %.3f avg_r %.3f maxq %.3f avg_maxq %.3f' % (
            e, t, time.time() - start_time, len(mem.memory), cur_eps, ep_len,
            R[e % args.window], np.mean(R),
            M[e % args.window], np.mean(M),
        ))

    return R


def main():
    parser = argparse.ArgumentParser(description='DQN')
    parser.add_argument('--env', choices=ENVS.keys())
    parser.add_argument('--max_episodes', type=int, default=10000)
    parser.add_argument('--gamma', type=float, default=0.99)
    parser.add_argument('--eps', type=float, default=1.0)
    parser.add_argument('--eps_decay_window', type=int, default=1000000)
    parser.add_argument('--eps_min', type=float, default=0.1)
    parser.add_argument('--clip', type=float, default=1)
    parser.add_argument('--window', type=int, default=100)
    parser.add_argument('--mem_capacity', type=int, default=1000000)
    parser.add_argument('--mem_init_size', type=int, default=50000)
    parser.add_argument('--batch_size', type=int, default=32)
    parser.add_argument('--sync_period', type=int, default=10000)
    parser.add_argument('--learn_freq', type=int, default=4)
    parser.add_argument('--save_freq', type=int, default=100)
    parser.add_argument('--gc_freq', type=int, default=100)
    parser.add_argument('--seed', type=int, default=0)
    parser.add_argument('--upload', action='store_true', default=False)
    args = parser.parse_args()

    work_dir = mkdir(EXP_DIR, args.env + '3')
    monitor_dir = mkdir(work_dir, 'monitor')

    E = gym.make(ENVS[args.env])

    # set seed
    E.seed(args.seed)
    np.random.seed(args.seed)
    torch.manual_seed(args.seed)
    torch.set_default_tensor_type('torch.cuda.FloatTensor')

    if args.upload:
        E = wrappers.Monitor(E, monitor_dir, force=True)

    dqn(E, args, work_dir)

    E.close()
    if args.upload:
        gym.upload(monitor_dir, api_key=API_KEY)


if __name__ == '__main__':
    main()