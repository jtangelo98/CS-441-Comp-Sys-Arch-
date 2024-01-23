#include <iostream>

using namespace std;

int main()
{
    string alphabet = "abc";
    int state_cnt = 4;
    int start = 1;
    bool final[] = {false, true, false, true, false};
    int cur = start;
    int trans[5][3] = {{0,0,0}, {2,0,0}, {0,3,2}, {1,0,4}, {3,1,3}};
    char *s = "abaab";

    while (*s != '\0' && cur != 0)
    {
        cur = trans[cur][alphabet.find(*s)];
        s++;
    }

    if (final[cur])
    {
        cout << "\n\ngood\n\n";
    }
    else
    {
        cout << "\n\nbad\n\n";
    }

    return 0;
}