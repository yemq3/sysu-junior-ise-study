import sys
import gym
import torch
import pylab
import random
import time
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
env.render()

state_size = env.observation_space.shape
action_size = 4
rewards, episodes = [], []

agent = Agent(action_size, load_model=True)
agent.epsilon = 0.01
frame = 0

for e in range(EPISODES):
    done = False
    score = 0

    history = np.zeros([5, 84, 84], dtype=np.uint8)
    step = 0
    state = env.reset()
    get_init_state(history, state)

    while not done:
        step += 1
        env.render()

        # Select and perform an action
        action = agent.get_action(np.float32(history[:4, :, :]) / 255.)

        
        next_state, reward, done, info = env.step(action)

        frame_next_state = get_frame(next_state)
        history[4, :, :] = frame_next_state
        history[:4, :, :] = history[1:, :, :]
        score += reward