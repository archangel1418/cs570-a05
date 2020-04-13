#include "mizzo.h"

struct Candy
{
    string name;
};

Candy createCandy()
{
    Candy newCandy = {.name = NULL};
    /*
        srand(time(0));
        double  r = ((double) rand() / (RAND_MAX)) + 1;

        if (r== 0){
            newCandy.name = "escargot suckers";
        }
        else{
            newCandy.name = "froggy bites";
        }
        */
    return newCandy;
}

int main(int argc, char *argv[])
{
    Candy one = createCandy();
    cout << one.name << endl;
}
