#include <iostream>
#include <queue>

using namespace std;


bool** fires;
int** points;
int** iterations;

struct coordinate
{
    coordinate(int x, int y) { X = x; Y = y; }
    coordinate() { X = -1; Y = -1; }
    int X;
    int Y;
};
coordinate operator+(coordinate c1, coordinate c2) { return coordinate(c1.X + c2.X, c1.Y + c2.Y); }

int iteration = 0;
int curr_queue = 0;

queue<coordinate> queues[2];

void add_cell(coordinate source, coordinate coord)
{
    if (points[coord.Y][coord.X] < points[source.Y][source.X])
    {
        if (points[source.Y][source.X] - fires[coord.Y][coord.X] < 0)
            return;
        if (iterations[coord.Y][coord.X] < iteration)
        {
            iterations[coord.Y][coord.X] = iteration;
            queues[(curr_queue + 1) % 2].push(coord);
        }
        points[coord.Y][coord.X] = points[source.Y][source.X] - fires[coord.Y][coord.X];
    }
}

void add_cells_around(coordinate coord)
{
    add_cell(coord, coordinate(coord.X, coord.Y + 1));
    add_cell(coord, coordinate(coord.X + 1, coord.Y));
    add_cell(coord, coordinate(coord.X, coord.Y - 1));
    add_cell(coord, coordinate(coord.X - 1, coord.Y));
}

int main()
{
    int Height, Width, F;
    int sX, sY, eX, eY;
    cin >> Height >> Width >> F >> sY >> sX >> eY >> eX;
    
    points = new int *[Height+2]();
    for (int i = 0; i < Height+2; i++)
    {
        points[i] = new int[Width + 2];
    }
    for (int i = 0; i < Width+2; i++)
    {
        points[0][i] = F;
        points[Height + 1][i] = F;
    }
    for (int i = 0; i < Height+2; i++)
    {
        points[i][0] = F;
        points[i][Width + 1] = F;
    }
    fires = new bool* [Height+2]();
    iterations = new int* [Height + 2]();
    for (int i = 1; i < Height+1; i++)
    {
        fires[i] = new bool[Width+2]();
        iterations[i] = new int[Width + 2]();
        for (int j = 1; j < Width+1; j++)
        {
            cin >> fires[i][j];
            points[i][j] = -1;
            iterations[i][j] = -1;
        }
    }
    
    queues[curr_queue].push(coordinate(sX, sY));
    points[sY][sX] = F;
    while (1)
    {
        while (!queues[curr_queue].empty())
        {
            coordinate curr = queues[curr_queue].front();
            queues[curr_queue].pop();
            if (curr.X == eX && curr.Y == eY)
            {
                cout << iteration << "\n";
                for (size_t i = 0; i < Height+2; i++)
                {
                    delete[] points[i]; 
                    delete[] fires[i];
                    delete[] iterations[i];
                }
                delete[] points;
                delete[] fires;
                delete[] iterations;
                return 0;
            }
            add_cells_around(curr);
        }
        if (queues[0].empty() && queues[1].empty())
            break;
        curr_queue = (curr_queue + 1) % 2;
        iteration++;
    }
    cout << "-1\n";
    for (size_t i = 0; i < Height + 2; i++)
    {
        delete[] points[i];
        delete[] fires[i];
        delete[] iterations[i];
    }
    delete[] points;
    delete[] fires;
    delete[] iterations;
    return 0;
}