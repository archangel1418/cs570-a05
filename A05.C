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
    for (int i = 0; i < BELTSIZE; i++)
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
    while (indexPtr->produceCount < 99)
    {
        //usleep(5);
        
       
        Candy nextCandy = createCandy();

        //keep track of number of froggy bites on belt at one time
        indexPtr->frogcounter = 0;
        for (int j = 0; j < BELTSIZE; j++)
        {
            if (belt[j].name == "froggy bites")
            {
                indexPtr->frogcounter++;
            }
        }

        if (indexPtr->frogcounter == 3)
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
        indexPtr->produceCount++;
		if (belt[indexPtr->beltIndex].name == "froggy bites"){
				indexPtr->totalFrogs++;
		}
		else{
			indexPtr->totalE++;
		}

        indexPtr->escargotCount = getCandyCount(belt) - indexPtr->frogcounter;

        sem_wait(&print);
        cout << ". Produced: " << indexPtr->produceCount
             << " Added " << belt[indexPtr->beltIndex].name << 
             "at index: " << indexPtr->beltIndex <<"\nBelt:" << endl;
        sem_post(&print);
        sem_wait(&print);
        for(int i =0; i<BELTSIZE; i++){
        	cout <<  i << ": " << belt[i].name << endl;	
        }
		sem_post(&print);
		indexPtr->beltIndex = (indexPtr->beltIndex +1) % BELTSIZE;
        //notifiy the end of this process
        sem_post(&mutex1);
        sem_post(&ItemsOnBelt);
    }
    pthread_exit(0);
}

void *consume(void *index)
{
    struct IndexManager *indexPtr;
    indexPtr = (struct IndexManager *)index;
    //int x=0;

    while (indexPtr->conCount < 99)
    {
    	sem_wait(&print);
    	if (indexPtr->name=="Ethel"){
    		indexPtr->name = "Lucy";
    		cout << "switch to L" << endl;
    	}
    	else if (indexPtr->name== "Lucy"){
    		indexPtr->name = "Ethel";
    		cout << "switch to E" << endl;
    	}
    	sem_post(&print);
        //check the number of items on the belt are >0 if so it will enter and decrement the #
        sem_wait(&ItemsOnBelt);
        //will check if mutex is greater than >0 if so it will enter and decrement 0
        sem_wait(&mutex1);
        

        //switch between Lucy and Ethel consuming candies
        /*
        if (indexPtr->switchConsumer = 0)
        {
            indexPtr->name = "Lucy";
            indexPtr->switchConsumer++;
        }
        else
        {
            indexPtr->name = "Ethel";
            indexPtr->switchConsumer--;
        }
        */

        //inc candy count based on person
        Candy temp;
        temp.name = belt[indexPtr->beltIndex].name;
        if (indexPtr->name == "Lucy")
        {
            if (temp.name == "froggy bites")
            {
                indexPtr->lucyFrogConsume++;
            }
            else
            {
                indexPtr->lucyEscargotConsume++;
            }
            indexPtr->lucyTotalConsume++;
        }
        else
        {
            if (temp.name == "froggy bites")
            {
                indexPtr->ethelFrogConsume++;
            }
            else
            {
                indexPtr->ethelEscargotConsume++;
            }
            indexPtr->ethelTotalConsume++;
        }

        

        //get amount of froggy bites after consumption
        for (int j = 0; j < BELTSIZE; j++)
        {
            if (belt[j].name == "froggy bites")
            {
                indexPtr->frogcounter--;
            }
        }
        //get amount of escargot suckers after consumption
        int escargotCount = getCandyCount(belt) - indexPtr->frogcounter;

        

        //remove candy
        belt[indexPtr->beltIndex].name = "";
        indexPtr->conCount++;
        
        sem_wait(&print);
        cout << "Consumed: " << indexPtr->conCount
             << " " << indexPtr->name
             << " consumed " << belt[indexPtr->beltIndex].name
             << "index: " << indexPtr->beltIndex << "\nBelt:";
        sem_post(&print);
        
        
        
        sem_wait(&print);
        for(int i =0; i<BELTSIZE; i++){
        	cout <<  i << ": " << belt[i].name << endl;	
        }
        sem_post(&print);
        indexPtr->beltIndex = (indexPtr->beltIndex + 1) % BELTSIZE;

        //increment
        sem_post(&mutex1);
        sem_post(&OpenSpaceOnBelt);
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

    pthread_t prothread1, prothread2, cthread, Ethread;

    // initialize mutex, its a binary mutex so it is either going to be 0 or 1
    sem_init(&mutex1, 0, 1);
    //init the size of ItemsOnBelt, starts at 0 because nothing has been addem
    sem_init(&ItemsOnBelt, 0, 0);
    //init size of Open spots, starts at buffer size cuz all space is available
    sem_init(&OpenSpaceOnBelt, 0, BELTSIZE);
    //init size of print, make this a binary semaphore
    sem_init(&print, 0, 1);

    //init struct and set attr for produce thread
    struct IndexManager producePlaceholder;
    producePlaceholder.beltIndex = 0;
    producePlaceholder.produceCount = 0;
    producePlaceholder.frogcounter = 0;

    //init struct and set attr for consume thread
    struct IndexManager consumePlaceholder;
    consumePlaceholder.beltIndex = 0;
    consumePlaceholder.conCount = 0;

    int r1 = pthread_create(&prothread1, NULL, produce, (void *)&producePlaceholder);
    int r2 = pthread_create(&prothread2, NULL, produce, (void *)&producePlaceholder);
	
	for(int k =0; k<=1; k++){
		if(k==0){
			consumePlaceholder.name = "Ethel";
		}
		else{
			consumePlaceholder.name = "Lucy";
		}
		pthread_create(&cthread, NULL, consume, (void *)&consumePlaceholder);
	}
	
	/*
    int r3 = pthread_create(&Lthread, NULL, consume, (void *)&consumePlaceholder);
    int r4 = pthread_create(&Ethread, NULL, consume, (void *)&consumePlaceholder);
    */

    pthread_join(prothread1, NULL);
    pthread_join(prothread2, NULL);
    pthread_join(cthread, NULL);
    //pthread_join(Ethread, NULL);

    sem_destroy(&mutex1);
    sem_destroy(&ItemsOnBelt);
    sem_destroy(&OpenSpaceOnBelt);
	
	sem_wait(&print);
    cout << "PRODUCTION REPORT" << '\n'
         << "----------------------------------------" << '\n'
         << "Crunchy frog bite producer generated "
         << producePlaceholder.totalFrogs
         << " candies \n"
         << "Escargot sucker producer generated "
         << producePlaceholder.totalE
         << " candies \n"
         << "Lucy consumed " << consumePlaceholder.lucyFrogConsume
         << " crunchy frog bites + "
         << consumePlaceholder.lucyEscargotConsume
         << " escargot suckers = " << consumePlaceholder.lucyTotalConsume
         << '\n'
         << "Ethel consumed "
         << consumePlaceholder.ethelFrogConsume
         << " crunchy frog bites + "
         << consumePlaceholder.ethelEscargotConsume
         << " escargot suckers = " << consumePlaceholder.ethelTotalConsume
         << endl;
     sem_post(&print);
     sem_destroy(&print);
}
