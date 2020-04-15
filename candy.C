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

/*
int main(int argc, char *argv[]){
    for (int i= 0; i<=10; i++){
        Candy newCandy = createCandy();
        cout << newCandy.name << endl;
    }
}
*/