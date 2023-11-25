import random, time
from typing import List

import numpy as np

import ox

class EpsGreedyBandit:
	def __init__(self, board):
		self.epsilon = 0.2
		self.actions = board.get_actions()
		self.qs = np.zeros(len(self.actions))
		self.visits = np.zeros(len(self.actions))
		self.last_idx = None

	def update(self, value: float) -> None:
		self.visits[self.last_idx] += 1
		self.qs[self.last_idx] += (value - self.qs[self.last_idx]) \
								  / self.visits[self.last_idx]

	def select(self):
		if np.random.rand() < self.epsilon:
			self.last_idx = np.random.randint(len(self.actions))
		else:
			self.last_idx = np.argmax(self.qs)
		return list(self.actions)[self.last_idx]

	def best_action(self):
		return list(self.actions)[np.argmax(self.qs)]

def random_simulation(board):
	while not board.is_terminal():
		random_action = np.random.choice(list(board.get_actions()))
		board.apply_action(random_action)

	return board.get_rewards()

class MCTSBot:
	def __init__(self, play_as: int, time_limit: float):
		self.play_as = play_as
		self.time_limit = time_limit * 0.9
		self.node_table = {}

	def has_node(self, board) -> bool:
		return board in self.node_table

	def get_node(self, board) -> EpsGreedyBandit:
		return self.node_table[board]

	def make_node(self, board):
		self.node_table[board] = EpsGreedyBandit(board)

	def best_action(self, board):
		return self.get_node(board).best_action()

	def expand(self, board) -> None:
		self.make_node(board)

	def backpropagate(self, trace, values: List[float]) -> None:
		for board in reversed(trace):
			self.get_node(board).update(values[board.current_player()])

	def select(self, board):
		trace = []
		while self.has_node(board):
			if board.is_terminal():
				break
			trace.append(board.clone())
			bandit = self.get_node(board)
			action = bandit.select()
			board.apply_action(action)
		return trace, board.clone()

	def play_action(self, board):
		start_time = time.time()
		while (time.time() - start_time) < self.time_limit:
			if not self.has_node(board):
				self.make_node(board)
			trace, last_state = self.select(board.clone())
			if last_state.is_terminal():
				values = last_state.get_rewards()
			else:
				self.expand(last_state)
				values=random_simulation(last_state.clone())
			self.backpropagate(trace, values)

		return self.best_action(board)





if __name__ == '__main__':
	board = ox.Board(8)  # 8x8
	bots = [MCTSBot(0, 0.1), MCTSBot(1, 1.0)]

	# try your bot against itself
	while not board.is_terminal():
		current_player = board.current_player()
		current_player_mark = ox.MARKS_AS_CHAR[ox.PLAYER_TO_MARK[current_player]]

		current_bot = bots[current_player]
		a = current_bot.play_action(board)
		board.apply_action(a)

		print(f"{current_player_mark}: {a} -> \n{board}\n")
