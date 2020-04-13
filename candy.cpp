#include "mizzo.h"

struct Candy
{
    string name;
    Candy createCandy();
};

Candy createCandy()
{
    Candy *newCandy = new Candy;
    
        srand(time(NULL));
        int  r = rand() % 2;
        cout << r << endl;

        if (r== 0){
            newCandy->name = "escargot suckers";
        }
        else{
            newCandy->name = "froggy bites";
        }
        
    return *newCandy;
}

