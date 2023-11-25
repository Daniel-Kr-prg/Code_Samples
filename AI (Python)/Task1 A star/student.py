from blockworld import BlockWorld
from queue import PriorityQueue


class BlockWorldHeuristic(BlockWorld):
    def __init__(self, num_blocks=5, state=None):
        BlockWorld.__init__(self, num_blocks, state)

    def heuristic(self, goal):
        self_state = self.get_state()
        goal_state = goal.get_state()

        wrong_box_poses = 0 # общее количество неправильных коробочек

        # for this_stack in self_state:
        #     found_stack = False # нашёлся ли в goal_state стак с таким же основанием как у this_stack
        #     for goal_stack in goal_state:
        #         if goal_stack[-1] == this_stack[-1]: #нашлось основание такое же
        #             found_stack = True
        #             for i in range(min(len(goal_stack), len(this_stack))):
        #                 if goal_stack[-i - 1] != this_stack[-i - 1]:
        #                     for j in range(len(this_stack)):
        #                         wrong_box_poses += len(this_stack) - j
        #         if found_stack:
        #             break
        # return wrong_box_poses * 2

        for this_stack in self_state:
            wrong_in_stack = len(this_stack) # предполагаю что в this_stack они щас все неправильные
            found_stack = False # нашёлся ли в goal_state стак с таким же основанием как у this_stack
            for goal_stack in goal_state:
                if goal_stack[-1] == this_stack[-1]: #нашлось основание такое же
                    found_stack = True
                    for i in range(min(len(goal_stack), len(this_stack))):
                        if goal_stack[-i - 1] == this_stack[-i - 1]:
                            wrong_in_stack -= 1 #убираем количество неправильных коробочек
                        else:
                            break
                if found_stack:
                    break
            wrong_box_poses += wrong_in_stack #прибавляем к общему
        return wrong_box_poses

class AStar():
    def search(self, start, goal):

        path_to = {start: [None, None]} # action , prev_state | типа closed лист

        opened = PriorityQueue()
        for action, neighbor in start.get_neighbors():
            opened.put((neighbor.heuristic(goal) + 1, action, neighbor, start, 1)) # f(x), action, state, prev_state, g(x)

        while opened:
            priority, action, neighbor, prev, step = opened.get()
            if neighbor in path_to:
                continue

            if neighbor == goal:
                ng = [action, prev]
                actions = []
                while ng[0] is not None:
                    actions.append(ng[0])
                    ng = path_to[ng[1]]
                actions.reverse()
                return actions

            path_to[neighbor] = [action, prev]

            for action_new, neighbor_new in neighbor.get_neighbors():
                opened.put((neighbor_new.heuristic(goal) + step, action_new, neighbor_new, neighbor, step+1))

        return None


if __name__ == '__main__':
    # Here you can test your algorithm. You can try different N values, e.g. 6, 7.
    N = 9

    start = BlockWorldHeuristic(N)
    goal = BlockWorldHeuristic(N)

    print("Searching for a path:")
    print(f"{start} -> {goal}")
    print()

    astar = AStar()
    path = astar.search(start, goal)

    if path is not None:
        print("Found a path:")
        print(path)

        print("\nHere's how it goes:")

        s = start.clone()
        print(s)

        for a in path:
            s.apply(a)
            print(s)

    else:
        print("No path exists.")

    print("Total expanded nodes:", BlockWorld.expanded)
