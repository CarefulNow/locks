#include <iostream>
#include <pthread.h>


typedef struct
{
    int val;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} semaphore;

semaphore s;


//printer qeues
void* printer (void *threadId);
void down (semaphore *s);
void up (semaphore *s);
void init (semaphore *s);


//amount to be printed
#define SIZE  50
pthread_t threadId[SIZE];


//scheduler
int main()
{
    int rc;
    int i;
    int j;

    srand(time(0));
    init(&s);


//create threads/items to print
    for (j = 0; j < SIZE; j++)
    {
        rc = pthread_create(&threadId[i], NULL, printer, NULL);
    }



//exit all existing threads
    for (i = 0; i < SIZE; i++)
    {
        int pthread_exit(0);
    }


    return 0;
}


void *printer (void *threadId)
{
    down(&s);
    printf("Printer %d\n", (rand() % 3 + 1));
    up(&s);

    return 0;
}

//decrement when being used
void down (semaphore *s)
{
    pthread_mutex_lock(&(s -> mutex));

//stops use of printer when none availible
    while (s -> val == 0)
    {
        pthread_cond_wait(&(s -> cond), &(s -> mutex));
    }

//decrements when mutex lock reacquired
    s -> val--;
    pthread_mutex_unlock(&(s -> mutex));
}

//increment when printer free
void up (semaphore *s)
{
    pthread_mutex_lock(&(s -> mutex));

    s -> val++;

//wake up other threads waiting on cond to be true
    pthread_cond_broadcast(&(s -> cond));
    pthread_mutex_unlock(&(s -> mutex));
}


void init (semaphore *s)
{
//number of printers
    s -> val = 3;
}
