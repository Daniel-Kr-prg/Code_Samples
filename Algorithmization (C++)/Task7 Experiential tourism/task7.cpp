
#include <iostream>
#include <queue>
#include <stack>

using namespace std;

int R, T, P;



struct queueElem {
    queueElem(int _col, int _p, int _e) :
        col(_col),
        p(_p),
        e(_e)
    {
    }
    int col;
    int e;
    int p;
};

vector<vector<queueElem>> ways;
int* paid_min;

int** result;

int maxInt;
queue<queueElem> q;

vector<int> topsorted;

void topsort(int vertex, bool visited[])
{
    visited[vertex] = true;
    for (int i = 0; i < ways[vertex].size(); i++)
    {
        if (!visited[ways[vertex][i].col])
        {
            topsort(ways[vertex][i].col, visited);
        }
    }

    topsorted.insert(topsorted.begin(), vertex);
}

void getTopSort()
{
    bool* visited = new bool[R + 1];
    for (int i = 0; i < R + 1; i++)
        visited[i] = false;

    for (int i = 1; i < R + 1; i++)
        if (visited[i] == false)
        {
            topsort(i, visited);
        }
}

void getMins()
{
    vector<int>::iterator iter;
    for (iter = topsorted.begin(); iter != topsorted.end(); iter++)
    {
        int curr = *iter;
        for (int i = 0; i < ways[curr].size(); i++)
        {
            int minn = paid_min[curr] + ways[curr][i].p;
            if (paid_min[ways[curr][i].col] > minn)
            {
                paid_min[ways[curr][i].col] = minn;
            }

        }
    }
}

void magic()
{
    vector<int>::iterator iter;
    for (iter = topsorted.begin(); iter != topsorted.end(); iter++)
    {
        int curr = *iter;
        //cout << "watching from " << curr << "\n";
        for (int i = 0; i < ways[curr].size(); i++)
        {
            for (int j = paid_min[curr]; j + ways[curr][i].p <= P; j++)
            {
                if (result[curr][j] == -1)
                {
                    continue;
                }
                
                int sum = result[curr][j] + ways[curr][i].e;
                //cout << curr << " <-- " << ways[curr][i].col << ": sum = " << sum << ", prev = " << result[ways[curr][i].col][ways[curr][i].p + j] << "P = " << j + ways[curr][i].p << "\n";
                if (sum > result[ways[curr][i].col][ways[curr][i].p + j])
                {
                    result[ways[curr][i].col][ways[curr][i].p + j] = sum;
                    if (j + ways[curr][i].p + paid_min[ways[curr][i].col] <= P)
                    maxInt = max(sum, maxInt);
                }
            }
        }
    }
}

int main() {

    cin >> R >> T >> P;

    ways = vector<vector<queueElem>>(R+1);
    paid_min = new int[R+1];
    result = new int* [R + 1];
    for (int i = 0; i <= R; i++)
    {
        paid_min[i] = 99999;
        result[i] = new int[P + 1];
        for (int j = 0; j <= P; j++)
        {
            result[i][j] = -1;
        }
    }
    paid_min[1] = 0;
    result[1][0] = 0;
    for (int i = 0; i < T; i++)
    {
        int r1, r2, e, z;
        cin >> r1 >> r2 >> e >> z;
        ways[r2].push_back(queueElem(r1, z, e));
    }
    maxInt = 0;
    
    getTopSort();
    getMins();
    magic();
    cout << maxInt << "\n";

    delete[] paid_min;
}