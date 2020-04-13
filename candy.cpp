#include "mizzo.h"

struct Candy
{
    string name;
};

Candy createCandy()
{
    Candy *newCandy = new Candy;
    
        srand(time(NULL));
        int  r = (rand() / (RAND_MAX)) + 1;
        cout << r << endl;

        if (r== 0){
            newCandy->name = "escargot suckers";
        }
        else{
            newCandy->name = "froggy bites";
        }
        
    return *newCandy;
}

int main(int argc, char *argv[])
{
    Candy one = createCandy();
    cout << one.name << endl;
}
