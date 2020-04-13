#include <semaphore.h>
#include <pthread.h>

#define BUFFERSIZE 10

//mutex variable to control the buffer access
sem_t mutex;
//will keep track of the number of items in the buffer
sem_t ItemsInBuffer;
//will track the number of spaces avaialable in the buffer
sem_t OpenSpotsInBuffer;

char buf[BUFFERSIZE];

int main(int argc, char *argv[]){
    pthread_t thread1, thread2;

    // initialize mutex, its a binary mutex so it is either going to be 0 or 1
    sem_init(&mutex, 0, 1);
    //init the size of ItemsInBuffer, starts at 0 because nothing has been addem
    sem_init(&ItemsInBuffer, 0, 0);
    //init size of Open spots, starts at buffer size cuz all space is available
    sem_init(&OpenSpotsInBuffer, 0, BUFFERSIZE);

    
}

void *producer(int *index){
    //intitalize the start //this should be where we produce the item
    int * startIndex = index;
    int currProduceIndex = *startIndex;
    while(true){
        // protect from overflow and control buffer
        sem_wait(&OpenSpotsInBuffer);
        sem_wait(&mutex);
        //add item to buffer
        buf[currProduceIndex] = currProduceIndex;

        //notifiy the end of this process
        sem_post(&mutex);
        sem_post(&ItemsInBuffer);
        currProduceIndex = (currProduceIndex+1) % BUFFERSIZE;
    }
}

void *consumer(int *index){
        int * startIndex = index;
        int currentConsumeIndex = *startIndex;
        while(true){
            sem_wait(&ItemsInBuffer);
            sem_wait(&mutex);
            //remove the candy here from the buffer
            sem_post(&mutex);
            sem_post(&OpenSpotsInBuffer);
            currentConsumeIndex = (currentConsumeIndex +1) % BUFFERSIZE;

        }

}
