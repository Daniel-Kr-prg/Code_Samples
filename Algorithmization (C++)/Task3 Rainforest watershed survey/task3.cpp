#include <iostream>
using namespace std;

class RiverPoint {
    public:
        RiverPoint()
            : keyLeft(-1)
            , keyRight(-1)
            , left(-1)
            , right(-1)
        {
        }
        RiverPoint(int left, int keyL, int right, int keyR)
            : keyLeft(keyL)
            , keyRight(keyR)
            , left(left)
            , right(right)
        {
        }

        int keyLeft;
        int keyRight;
        int left;
        int right;
};

RiverPoint* points;

void getPointValue(int index, int* closedValue, int* openedValue)
{
    if (points[index].left == -1 || points[index].right == -1)
    {
        return;
    }
    int closedLeft = 0, openedLeft = 0, closedRight = 0, openedRight = 0;

    getPointValue(points[index].left, &closedLeft, &openedLeft);
    getPointValue(points[index].right, &closedRight, &openedRight);

    *closedValue = max(closedLeft + closedRight, openedLeft + openedRight + points[index].keyLeft + points[index].keyRight);
    *openedValue = max(openedLeft + points[index].keyLeft + closedRight, openedRight + points[index].keyRight + closedLeft);
}

int main()
{
    int startIndex, P, S;
    cin >> P >> S;
    points = new RiverPoint[P+S]();

    int vars[5];
    cin >> vars[0] >> vars[1] >> vars[2] >> vars[3] >> vars[4];
    startIndex = vars[0];
    points[vars[0]] = RiverPoint(vars[1], vars[2], vars[3], vars[4]);
    for (int i = 0; i < S - 1; i++)
    {
        cin >> vars[0] >> vars[1] >> vars[2] >> vars[3] >> vars[4];
        points[vars[0]] = RiverPoint(vars[1], vars[2], vars[3], vars[4]);
    }
    int closed = 0, opened = 0;
    getPointValue(startIndex, &closed, &opened);

    cout << closed << "\n";
}