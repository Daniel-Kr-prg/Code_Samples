#include <iostream>
#include <vector>



using namespace std;


class Node
{
public:
    Node()
        : connected(0), connectedCount(0), leftVal(0), rightVal(0)
    {
    }

    vector<int> connected;
    int connectedCount;

    long leftVal;
    long rightVal;
};

int S,K,M;

Node * nodes;
//char ** sequences;

long ** table;

long long totalSum = 0;



void FillTable(int n)
{
    for (int i = 0; i < S; i++)
    {
        table[i][n] = 0;
        for (vector<int>::iterator j = nodes[i].connected.begin(); j != nodes[i].connected.end(); j++)
        {
            int d = *j;
            table[i][n] += table[d][n-1];
        }
    }
}

void BuildGraph()
{
    for (int i = 0; i < S; i++)
    {
        for (int j = 0; j < S; j++)
        {
            if (nodes[i].rightVal == nodes[j].leftVal)
            {
                nodes[i].connected.push_back(j);
                nodes[i].connectedCount += 1;
            }
            
        }
        
    }
    
}

int main()
{
    cin >> S >> K >> M;

    nodes = new Node[S];

    table = new long * [S];

    for (int i = 0; i < S; i++)
    {
        table[i] = new long[M-K];

        nodes[i] = Node();

        char tmp;
        cin >> tmp;
        tmp -= 64;
        nodes[i].leftVal = nodes[i].leftVal * 10 + tmp;

        for (int j = 0; j < K-2; j++)
        {
            cin >> tmp;
            tmp -= 64;
            nodes[i].leftVal = nodes[i].leftVal * 10 + tmp;
            nodes[i].rightVal = nodes[i].rightVal * 10 + tmp;
        }
        
        cin >> tmp;
        tmp -= 64;
        nodes[i].rightVal = nodes[i].rightVal * 10 + tmp;
    }

    
    
    BuildGraph();

    for (int i = 0; i < S; i++)
    {
        // cout << nodes[i].connectedCount << " ";
        table[i][0] = nodes[i].connectedCount;
    }
    // cout << "\n";
    for (int i = 1; i < M-K; i++)
    {
        FillTable(i);
    }

    // for (int i = 0; i < S; i++)
    // {
    //     for (int j = 0; j < M-K; j++)
    //     {
    //         cout << table[i][j] << " ";
    //     }
    //     cout << "\n";
    // }
    

    for (int i = 0; i < S; i++)
    {
        totalSum += table[i][M-K-1];
    }
    
    cout << totalSum << "\n";
    return 0;
}

