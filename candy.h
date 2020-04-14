#include <semaphore.h>
#include <pthread.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#define BELTSIZE 10

using namespace std;

struct Candy{
    string name;
    Candy createCandy();
};
void *produce(int *index);
void *consume(int *index);
