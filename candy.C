#include "candy.h"

Candy createCandy()
{
    uniform_int_distribution<int> d(0, 1);
    random_device rd;
    struct Candy newCandy;

    int r = d(rd);
    //cout << r << endl;

    if (r == 0)
    {
        newCandy.name = "escargot suckers";
    }
    else
    {
        newCandy.name = "froggy bites";
    }

    return newCandy;
}