#include <mizzo.h>
#include <stdlib.h>
#include <string>

using namespace std;

class Candy{
    public:
    string name;
    Candy createCandy();
    string getName(){
        return name;
    }
    
};

    Candy createCandy(){
        Candy newCandy;
        int rand = rand() % 1;

        if (rand== 0){
            newCandy.name = "escargot suckers";
        }
        else{
            newCandy.name = "froggy bites";
        }

        return newCandy;
    }
