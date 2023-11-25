import math

import numpy as np

from blockworld import BlockWorldEnv
import random


class QLearning():
	# don't modify the methods' signatures!
	def __init__(self, env: BlockWorldEnv):
		self.env = env
		self.alpha = 0.4
		self.discount_factor = 0.9
		self.epsilon = 0.4
		self.k = 10
		self.episode = 1
		self.q_table = {}

	def train(self):
		num_episodes = 10000
		for i in range(num_episodes):
			state, goal = self.env.reset()
			done = False
			while not done:
				# Choose an action
				if goal not in self.q_table:
					self.q_table[goal] = {}
				if state not in self.q_table[goal]:
					self.q_table[goal][state] = {}
					for action in state.get_actions():
						self.q_table[goal][state][action] = 0.0
				actions = state.get_actions()
				if not actions:
					self.q_table[goal][state] = {0: 0}
					break

				if random.uniform(0, 1) < self.epsilon:
					action = random.choice(actions)
				else:
					action = max(self.q_table[goal][state], key=self.q_table[goal][state].get)

				next_state, reward, done = self.env.step(action)

				# Update the Q-table
				if next_state[0] not in self.q_table[goal]:
					next_actions = next_state[0].get_actions()
					if not next_actions:
						self.q_table[goal][next_state[0]] = {0: 0}
					else:
						self.q_table[goal][next_state[0]] = {}
						for next_action in next_state[0].get_actions():
							self.q_table[goal][next_state[0]][next_action] = 0.0

				max_q_value = max(self.q_table[goal][next_state[0]].values()) if next_state[0] in self.q_table[
					goal] else 0
				self.q_table[goal][state][action] += self.alpha * (
						reward + self.discount_factor * max_q_value - self.q_table[goal][state][
					action])

				# Move to the next state
				state = next_state[0]
			self.episode += 1

	def get_alpha(self):
		# Task: use self.alpha_init, self.episode and self.k
		return min(self.alpha, self.alpha / (self.k * self.episode))

	def act(self, state):
		if state[1] not in self.q_table:
			self.q_table[state[1]] = {}
		if state[0] not in self.q_table[state[1]]:
			self.q_table[state[1]][state[0]] = {}
			for action in state[0].get_actions():
				self.q_table[state[1]][state[0]][action] = 0.0
		return max(self.q_table[state[1]][state[0]], key=self.q_table[state[1]][state[0]].get)


if __name__ == '__main__':
	# Here you can test your algorithm. Stick with N <= 4
	N = 4

	env = BlockWorldEnv(N)
	qlearning = QLearning(env)

	# Train
	qlearning.train()

	# Evaluate
	test_env = BlockWorldEnv(N)

	test_problems = 1000
	solved = 0
	avg_steps = []

	for test_id in range(test_problems):
		s = test_env.reset()
		done = False

		# print(f"\nProblem {test_id}:")
		# print(f"{s[0]} -> {s[1]}")

		for step in range(50):  # max 50 steps per problem
			a = qlearning.act(s)
			s_, r, done = test_env.step(a)

			# print(f"{a}: {s[0]}")

			s = s_

			if done:
				solved += 1
				avg_steps.append(step + 1)
				break

	avg_steps = sum(avg_steps) / len(avg_steps)
	print(f"Solved {solved}/{test_problems} problems, with average number of steps {avg_steps}.")
