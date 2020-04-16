#include "candy.h"

//mutex variable to control the buffer access
sem_t mutex1;
//will keep track of the number of items in the buffer
sem_t ItemsOnBelt;
//will track the number of spaces avaialable in the buffer
sem_t OpenSpaceOnBelt;
//will keep track of printing
sem_t print;

Candy belt[BELTSIZE];
int frogcounter;
int produceCount = 0;
bool stopProduce = true;
int conCount = 0;

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
        if (buff[i].name == "froggy bites" || buff[i].name == "escargot suckers")
        {
            candyOnBelt++;
        }
    }
    return candyOnBelt;
}

void *produce(void *index)
{
    struct IndexManager *indexPtr;
    indexPtr = (struct IndexManager *)index;

    //Loop production of candies till limit is reached
    while (produceCount < 15)
    {
        //usleep(5);
        Candy nextCandy = createCandy();
        if (frogcounter == 3)
        {
            nextCandy.name = "escargot suckers";
        }

        //protect from overflow and control buffer
        //will check if openSpaceOnBelt is >0 if so it will enter and decrement open spaces
        sem_wait(&OpenSpaceOnBelt);
        //will check if mutex is >=0 if so it will enter and decrement mutex
        sem_wait(&mutex1);
        //add item to buffer
        belt[indexPtr->beltIndex].name = nextCandy.name;
        produceCount++;

        //keep track of number of froggy bites on belt at one time
        if (nextCandy.name == "froggy bites")
        {
            frogcounter++;
        }

        sem_wait(&print);
        cout << "Produced: " << nextCandy.name << " Total Produced: " << produceCount << " at index: " << indexPtr->beltIndex << endl;
        sem_post(&print);

        //set where we are currently on the beltIndex
        indexPtr->beltIndex = (indexPtr->beltIndex + 1) % BELTSIZE;
		sem_wait(&print);
		cout << "Thread Index: " << indexPtr->beltIndex << endl;
		sem_post(&print);

        //notifiy the end of this process
        sem_post(&mutex1);
        sem_post(&ItemsOnBelt);
    }
    pthread_exit(0);
}

void *consume(void *index)
{
    //int candyCount = getCandyCount(belt);
    int *startIndex = static_cast<int *>(index);
    int conIndex = *startIndex;
    while (conCount < 15)
    {
        //candyCount = getCandyCount(belt);
        //cout << candyCount << endl;
        //*startIndex = getConsumeIndex(belt);
        //check the number of items on the belt are >0 if so it will enter and decrement the #
        sem_wait(&ItemsOnBelt);
        //will check if mutex is greater than >0 if so it will enter and decrement 0
        sem_wait(&mutex1);
        //check if candy we are removing is a frog if so decrement counter
        Candy temp = belt[conIndex];
        if (temp.name == "froggy bites")
        {
            frogcounter--;
        }
        //remove candy
        belt[conIndex].name = "";
        conCount++;
        sem_wait(&print);
        cout << "Consumed: " << temp.name << "Total consumed: " << conCount << "at index: " << conIndex << endl;
        int count = getCandyCount(belt);
        cout << "candies on belt: " << count << endl;
        sem_post(&print);
        //remove candy from belt
        //increment
        sem_post(&mutex1);
        sem_post(&OpenSpaceOnBelt);
        //candyCount = getCandyCount(belt);
        conIndex = (conIndex + 1) % BELTSIZE;
        //conIndex = getConsumeIndex(belt);
    }
    pthread_exit(0);
}

int main(int argc, char *argv[])
{
    //Handles flags from command line
    char *hold;
    int timeDelay = 0;

    for (int k = 1; k < argc; k++)
    {
        if ((strncmp(argv[k], "-E", 2)) == 0)
        {
            hold = argv[k + 1];
            timeDelay = stoi(hold);
        }

        if ((strncmp(argv[k], "-L", 2)) == 0)
        {
            hold = argv[k + 1];
            timeDelay = stoi(hold);
        }

        if ((strncmp(argv[k], "-f", 2)) == 0)
        {
            hold = argv[k + 1];
            timeDelay = stoi(hold);
        }

        if ((strncmp(argv[k], "-e", 2)) == 0)
        {
            hold = argv[k + 1];
            timeDelay = stoi(hold);
        }
    }

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
    //init size of print, make this a binary semaphore
    sem_init(&print, 0, 1);

    struct IndexManager producePlaceholder;
    producePlaceholder.beltIndex = 0;

	struct IndexManager consumePlaceholder;
    consumePlaceholder.beltIndex = 0;

    int r1 = pthread_create(&prothread1, NULL, produce, (void *)&producePlaceholder);
    int r2 = pthread_create(&prothread2, NULL, produce, (void *)&producePlaceholder);
    //produceCount++;
    int r3 = pthread_create(&Ethread, NULL, consume, (void *)&consumePlaceholder);
    //int r4 = pthread_create(&Lthread, NULL, consume, (void *)&consumePlaceholder);

    pthread_join(prothread1, NULL);
    //pthread_join(prothread2, NULL);
    pthread_join(Ethread, NULL);
    //pthread_join(conthread2, NULL);

    sem_destroy(&mutex1);
    sem_destroy(&ItemsOnBelt);
    sem_destroy(&OpenSpaceOnBelt);
}
