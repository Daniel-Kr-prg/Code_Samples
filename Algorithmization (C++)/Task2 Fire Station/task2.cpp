// task2.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <set>
#include <vector>

using namespace std;
struct elem {
	int v, x, y;
};

struct comparer {
	bool operator()(const elem& one, const elem& two) const { return one.v < two.v; }
};

multiset<elem, comparer> costs;
multiset<elem, comparer>::iterator end_it;

bool * blocked_X;
bool * blocked_Y;

elem * selected;

int H, W, D, N, total;

int bestReachedPoint = 9999;
int bestSum = 99999;

static bool isAbleToPlace(elem el, int currSel)
{
    if (blocked_X[el.x] || blocked_Y[el.y])
    {
		return false;
    }
    if (D > 2)
	    for (int i = 0; i <= currSel; i++)
	    {
            int val = abs(el.x - (*(selected + i)).x) + abs(el.y - (*(selected + i)).y);
            if (val < D)
		    {
			    return false;
		    }
	    }
	return true;
}

int tryPlace(multiset<elem, comparer>::iterator currIter, int currSelected, int currSum, int currPoint)
{
    blocked_X[(*currIter).x] = true;
    blocked_Y[(*currIter).y] = true;

    selected[currSelected] = (*currIter);
    auto newIter = next(currIter, 1);
    
    int pointsSkipped = 1;
    int stationsToPut = N - currSelected - 1;
    int r = 0;
    while (newIter != end_it)
    {
        if (bestReachedPoint <= currPoint + stationsToPut)
        {
            break;
        }
        if (currSum + (*newIter).v >= bestSum)
        {
            break;
        }
        if (isAbleToPlace(*newIter, currSelected))
        {
            if (currSelected == N - 2)
            {
                bestSum = currSum + (*newIter).v;
                bestReachedPoint = currPoint + pointsSkipped;
                blocked_X[(*currIter).x] = false;
                blocked_Y[(*currIter).y] = false;
                /*for (size_t i = 0; i < 9; i++)
                {
                    cout << "[" << selected[i].x << "," << selected[i].y << "] ";
                }
                cout << "[" << (*newIter).x << "," << (*newIter).y << "]\n ";*/
                return pointsSkipped;
            }
            if ((r = tryPlace(newIter, currSelected + 1, currSum + (*newIter).v, currPoint + pointsSkipped)) < pointsSkipped + stationsToPut)
            {

                break;
            }
        }
        newIter++;
        pointsSkipped++;
    }

    blocked_X[(*currIter).x] = false;
    blocked_Y[(*currIter).y] = false;

    
    return r + pointsSkipped;
}

void findSolution()
{
    auto newIter = costs.begin();
    end_it = costs.end();
    if (N == 1)
    {
        bestSum = (*newIter).v;
        return;
    }
    int points = 0;
    while (newIter != end_it)
    {
        if ((*newIter).v >= bestSum)
        {
            break;
        }
        else
        {
            tryPlace(newIter, 0, (*newIter).v, points);
            if (points + N >= bestReachedPoint)
            {
                break;
            }
        }
        newIter++;
        points++;
    }
}

int main()
{
    cin >> H >> W >> D >> N;

    blocked_X = new bool[W]();
    blocked_Y = new bool[H]();

    total = W * H;

    selected = new elem[N]();

    for (size_t i = 0; i < W; i++)
        blocked_X[i] = false;
    for (size_t i = 0; i < H; i++)
        blocked_Y[i] = false;
    auto itr = costs.begin();
    for (int i = 0; i < H; i++)
    {
        for (int j = 0; j < W; j++)
        {
            int val;
            cin >> val;
            itr = costs.insert(itr, elem { val, j, i });
        }
    }


    findSolution();
    cout << bestSum << "\n";

    delete[] blocked_X, blocked_Y, selected;

    return 0;
}
