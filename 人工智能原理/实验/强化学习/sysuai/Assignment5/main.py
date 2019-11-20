import sys
import gym
import torch
import pylab
import random
import numpy as np
from collections import deque
from datetime import datetime
from copy import deepcopy
import torch.nn as nn
import torch.optim as optim
import torch.nn.functional as F
from torch.autograd import Variable
from utils import *
from agent import *
from model import *
from config import *

env = gym.make('Breakout-v0')
# env.render()

number_lives = find_max_lifes(env)
state_size = env.observation_space.shape
action_size = 3
rewards, episodes = [], []

agent = Agent(action_size)
evaluation_reward = deque(maxlen=evaluation_reward_length)
frame = 0
memory_size = 0
save_threshold = 10

for e in range(EPISODES):
    done = False
    score = 0

    history = np.zeros([5, 84, 84], dtype=np.uint8)
    step = 0
    d = False
    state = env.reset()
    life = number_lives

    get_init_state(history, state)

    while not done:
        step += 1
        frame += 1
        # if render_breakout:
        #     env.render()

        # Select and perform an action
        action = agent.get_action(np.float32(history[:4, :, :]) / 255.)

        
        next_state, reward, done, info = env.step(action + 1)

        frame_next_state = get_frame(next_state)
        history[4, :, :] = frame_next_state
        terminal_state = check_live(life, info['ale.lives'])

        life = info['ale.lives']
        r = np.clip(reward, -1, 1)

        # Store the transition in memory 
        agent.memory.push(deepcopy(frame_next_state), action, r, terminal_state)
        # Start training after random sample generation
        if(frame >= train_frame):
            if(frame % Update_policy_network_frequency) == 0:
                agent.train_policy_net(frame)
            # Update the target network
            if(frame % Update_target_network_frequency)== 0:
                agent.update_target_net()
        score += reward
        history[:4, :, :] = history[1:, :, :]

        if frame % 50000 == 0:
            print('now time : ', datetime.now())
            rewards.append(np.mean(evaluation_reward))
            episodes.append(e)
            pylab.plot(episodes, rewards, 'b')
            pylab.savefig("/home/sysuai/workspace/17363092/Assignment5/save_graph/breakout_dqn_add_freq.png")

        if done:
            evaluation_reward.append(score)
            # every episode, plot the play time
            print("episode:", e, "  score:", score, "  memory length:",
                  len(agent.memory), "  epsilon:", agent.epsilon, "   steps:", step,
                  "    evaluation reward:", np.mean(evaluation_reward))

            # if the mean of scores of last 10 episode is bigger than 400
            # stop training
            mean_evaluation_reward = np.mean(evaluation_reward)
            if mean_evaluation_reward > save_threshold:
                file_path = "/home/sysuai/workspace/17363092/Assignment5/save_model/breakout_dqn_add_freq_{}".format(int(mean_evaluation_reward))
                torch.save(agent.policy_net, file_path)
                print("save model:", file_path)
                save_threshold += 10