#include "candy.h"
#include <semaphore.h>
#include <pthread.h>

//mutex variable to control the buffer access
sem_t mutex1;
//will keep track of the number of items in the buffer
sem_t ItemsOnBelt;
//will track the number of spaces avaialable in the buffer
sem_t OpenSpaceOnBelt;

Candy belt[BELTSIZE];
int frogcounter;
int produceCount = 0;
bool stopProduce = true;

//get index for producer by checking null in the belt array, return first index that is null
int getProduceIndex(Candy buff[])
{
    for (int i = 0; i < 10; i++)
    {
        if (buff[i].name == "")
        {
            return i;
        }
    }
    return -1;
}

//get index for consumer by checking for not null in the belt array, return first index that is not null
int getConsumeIndex(Candy buff[])
{
    for (int i = 0; i < 10; i++)
    {
        if (buff[i].name != "")
        {
            return i;
        }
    }
    return -1;
}

int getCandyCount(Candy buff[])
{
    int candyOnBelt = 0;
    for (int i = 0; i < 10; i++)
    {
        if (buff[i].name != "")
        {
            candyOnBelt++;
        }
    }
    return candyOnBelt;
}

void *produce(void *index)
{
    //pthread_t thread;
    int *startIndex = static_cast<int *>(index);
	int prodIndex = *startIndex;
    //*startIndex= getProduceIndex(belt);
    //intitalize the start //this should be where we create the item

    //checks if there are 3 froggy bites on the belt
    /*
    if (frogcounter>=3){
        nextCandy.name = "escargot suckers";
    }
    */
    //define starting index which is 0
    while (produceCount < 100)
    {
        Candy nextCandy = createCandy();
        //nextCandy= createCandy();
        // protect from overflow and control buffer
        //will check if openSpaceOnBelt is >0 if so it will enter and decrement open spaces
        sem_wait(&OpenSpaceOnBelt);
        //will check if mutex is >=0 if so it will enter and decrement mutex
        sem_wait(&mutex1);
        //add item to buffer
        belt[prodIndex].name = nextCandy.name;
        produceCount++;
        //keep track of number of froggy bites on belt at one time
        if (nextCandy.name == "froggy bites")
        {
            frogcounter++;
        }
        cout << "Produced: " << nextCandy.name << " Total Produced: " << produceCount << endl;
        //notifiy the end of this process
        sem_post(&mutex1);
        sem_post(&ItemsOnBelt);
        //dont think we need this increment anymore
        prodIndex = (prodIndex + 1) % BELTSIZE;
    }
    pthread_exit(0);
}

void *consume(void *index)
{
    //int candyCount = getCandyCount(belt);
    int *startIndex = static_cast<int *>(index);
	int conIndex = *startIndex;
    while (produceCount<100)
    {
        //candyCount = getCandyCount(belt);
        //cout << candyCount << endl;
        //*startIndex = getConsumeIndex(belt);
        //check the number of items on the belt are >0 if so it will enter and decrement the #
        sem_wait(&ItemsOnBelt);
        //will check if mutex is greater than >0 if so it will enter and decrement 0
        sem_wait(&mutex1);
        //check if candy we are removing is a frog if so decrement counter
        if (belt[conIndex].name == "froggy bites")
        {
            frogcounter--;
        }
        cout << "Consumed: " << belt[conIndex].name << endl;
        //remove candy from belt
        belt[conIndex].name = "";

        //increment
        sem_post(&mutex1);
        sem_post(&OpenSpaceOnBelt);
        //candyCount = getCandyCount(belt);
        conIndex = (conIndex + 1) % BELTSIZE;
    }
    pthread_exit(0);
}

int main(int argc, char *argv[])
{
    for (int i = 0; i < BELTSIZE; i++)
    {
        belt[i].name = "";
    }

    pthread_t prothread1, prothread2, Ethread, Lthread;

    // initialize mutex, its a binary mutex so it is either going to be 0 or 1
    sem_init(&mutex1, 0, 1);
    //init the size of ItemsOnBelt, starts at 0 because nothing has been addem
    sem_init(&ItemsOnBelt, 0, 0);
    //init size of Open spots, starts at buffer size cuz all space is available
    sem_init(&OpenSpaceOnBelt, 0, BELTSIZE);

    int produceIndex = 0;
    int consumerIndex = 0;

    int r1 = pthread_create(&prothread1, NULL, produce, (void *)&produceIndex);
    int r2 = pthread_create(&prothread2, NULL, produce, (void *)&produceIndex);
    //produceCount++;
    int r3 = pthread_create(&Ethread, NULL, consume, (void *)&consumerIndex);
    //int r4 = pthread_create(&conthread2, NULL, consume, (void *)&consumerIndex);

    pthread_join(prothread1, NULL);
    //pthread_join(prothread2, NULL);
    pthread_join(Ethread, NULL);
    //pthread_join(conthread2, NULL);

    sem_destroy(&mutex1);
    sem_destroy(&ItemsOnBelt);
    sem_destroy(&OpenSpaceOnBelt);
}
