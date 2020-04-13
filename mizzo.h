#include <semaphore.h>
#include <pthread.h>
#include <string>


#define BELTSIZE 10

void *produce(int *index);

void *consume(int *index);

struct Candy;