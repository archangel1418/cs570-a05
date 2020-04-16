#include <semaphore.h>
#include <pthread.h>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <random>
#include <unistd.h>
#include <time.h>

#define BELTSIZE 10

using namespace std;

struct IndexManager
{
    int beltIndex;
    int frogcounter;
    int produceCount;
    int conCount;
    string name;
};

struct Candy
{
    string name;
};
Candy createCandy();

int getProduceIndex(Candy buff[]);
int getConsumeIndex(Candy buff[]);
int getCandyCount(Candy buff[]);

void *produce(int *index);
void *consume(int *index);
