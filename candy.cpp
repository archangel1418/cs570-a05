#include "candy.h"

Candy createCandy()
{
    struct Candy newCandy;

    srand(time(NULL));
    int r = rand() % 2;
    cout << r << endl;

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