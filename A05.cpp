#include "mizzo.h"

//mutex variable to control the buffer access
sem_t mutex1;
//will keep track of the number of items in the buffer
sem_t ItemsOnBelt;
//will track the number of spaces avaialable in the buffer
sem_t OpenSpaceOnBelt;

Candy belt[BELTSIZE];

int main(int argc, char *argv[])
{
    pthread_t thread1, thread2;

    // initialize mutex, its a binary mutex so it is either going to be 0 or 1
    sem_init(&mutex1, 0, 1);
    //init the size of ItemsOnBelt, starts at 0 because nothing has been addem
    sem_init(&ItemsOnBelt, 0, 0);
    //init size of Open spots, starts at buffer size cuz all space is available
    sem_init(&OpenSpaceOnBelt, 0, BELTSIZE);
}

void *produce()
{
    //intitalize the start //this should be where we produce the item
    Candy nextCandy;
    while (true)
    {
        nextCandy.createCandy();
        // protect from overflow and control buffer
        sem_wait(&OpenSpaceOnBelt);
        sem_wait(&mutex1);
        //add item to buffer
        belt[currProduceIndex] = currProduceIndex;

        //notifiy the end of this process
        sem_post(&mutex1);
        sem_post(&ItemsOnBelt);
        currProduceIndex = (currProduceIndex + 1) % BELTSIZE;
    }
}

void *consume(int *index)
{
    int *startIndex = index;
    int currentConsumeIndex = *startIndex;
    while (true)
    {
        sem_wait(&ItemsOnBelt);
        sem_wait(&mutex1);
        //remove the candy here from the buffer
        sem_post(&mutex1);
        sem_post(&OpenSpaceOnBelt);
        currentConsumeIndex = (currentConsumeIndex + 1) % BELTSIZE;
    }
}
