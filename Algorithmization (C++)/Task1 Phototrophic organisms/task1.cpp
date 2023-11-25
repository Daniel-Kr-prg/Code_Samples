#include <iostream>
#include <vector>
using namespace std;

int main()
{
    int n;
    cin >> n;
    int d1, d2;
    cin >> d1;
    cin >> d2;

    int * times1 = new int[n]();
    int * times2 = new int[n]();

    cin >> *times1;
    for (int *i = times1+1; i != times1+n; i++)
    {
        cin >> *i;
        *i += *(i - 1);
    }
    cin >> *times2;
    for (int *i = times2+1; i != times2+n; i++)
    {
        cin >> *i;
        *i += *(i - 1);
    }

    int totalTime = *(times1 + n - 1);
    int minLight = 99999999;
    int maxLight = -1;

    for (int i = 0; i < n; i++)
    {
        int idx = i - d1 - 1;
        int bot1Enter = idx < 0 ? 0 : *(times1 + idx);
        int bot1Exit = *(times1 + min(i + d1, n - 1));

        int bot2Enter = totalTime - *(times2 + min(n - 1, i + d2));
        idx = i - d2 - 1;
        int bot2Exit = idx < 0 ? totalTime : totalTime - *(times2 + idx);

        int intersectStartTime = max(bot1Enter, bot2Enter);
        int intersectEndTime = min(bot1Exit, bot2Exit);
        
        int intersectTime = intersectEndTime - intersectStartTime;

        int time = 0;
        if (intersectTime > 0)
        {
            time = intersectTime * 3 + (max(bot1Exit, bot2Exit) - min(bot1Enter, bot2Enter) - intersectTime) * 2;
        }
        else
        {
            time = (bot1Exit - bot1Enter + bot2Exit - bot2Enter) * 2;
        }
        minLight = time < minLight ? time : minLight;
        maxLight = time > maxLight ? time : maxLight;
    }

    delete [] times1, times2;

    cout << minLight << " " << maxLight << "\n";
}
