#include <semaphore.h>
#include <pthread.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#define BELTSIZE 10

using namespace std;

void *produce(int *index);
void *consume(int *index);

Candy createCandy();

struct Candy
{
    string name;
};