#include <semaphore.h>
#include <pthread.h>

using namespace std;

#define BELTSIZE 10

void *produce(int *index);

void *consume(int *index);