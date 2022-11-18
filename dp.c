#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
// sem_t *sem_id1 = sem_open("semChop1", O_CREAT, O_RDWR, 10);
// sem_t *sem_id2 = sem_open("semChop2", O_CREAT, O_RDWR, 10);
// sem_t *sem_id3 = sem_open("semChop3", O_CREAT, O_RDWR, 10);
// sem_t *sem_id4 = sem_open("semChop4", O_CREAT, O_RDWR, 10);
// sem_t *sem_id5 = sem_open("semChop5", O_CREAT, O_RDWR, 10);

//a bool enum
typedef enum
{
    False,
    True
} boolean;

struct chopStick
{
    unsigned long name;
    unsigned long owner;
    boolean pickedUp;
    pthread_mutex_t lock;
};
struct chopStick chopStickArray[5];

//put down left chopstick
void putDownLeft(unsigned long name)
{
    chopStickArray[name].owner = 6;
    chopStickArray[name].pickedUp = False;
    pthread_mutex_unlock(&(chopStickArray[name].lock));
}

//put down right chopstick
void putDownRight (unsigned long name)
{
    chopStickArray[name + 1].owner = 6;
    chopStickArray[name + 1].pickedUp = False;
    pthread_mutex_unlock(&(chopStickArray[(name + 1) % 5].lock));
}

//eat
void eat (unsigned long name, boolean* eaten) 
{
    int randomNumber = 0;
    if (chopStickArray[name].owner == name && chopStickArray[name + 1].owner == name)
    {
        printf("Professor %lu got right -> eating\n", name);
        *eaten = True;
        randomNumber = rand() % 6 + 5;
        sleep(randomNumber);
        putDownLeft(name);
        putDownRight(name);
        printf("Professor %lu eating -> dropped both chopsticks\n", name);
    }
}

//picks up left chopstick
void pickLeft(unsigned long name)
{
    pthread_mutex_lock(&(chopStickArray[name].lock));
    if (chopStickArray[name].pickedUp == False)
    {
        chopStickArray[name].pickedUp = True;
        chopStickArray[name].owner = name;
        printf("Professor %lu thinking -> got left chopstick\n", name);
    }
}

//tries to pick up right chopstick
void pickRight(unsigned long name, boolean* eaten)
{
    if (chopStickArray[name + 1].pickedUp == False)
    {
        pthread_mutex_lock(&(chopStickArray[(name + 1) % 5].lock));
        printf("Professor %lu thinking -> got right chopstick\n", name);
        chopStickArray[name + 1].pickedUp = True;
        chopStickArray[name + 1].owner = name;
        eat(name, eaten);
    }
    else 
    {
        // Drop the left chopstick
        putDownLeft(name);
        printf("Professor %lu thinking -> dropped left -> thinking\n", name);
    }
}

void *professor(void *buf)
{
    unsigned long name = (unsigned long)buf;
    int randomNumber = 0;
    boolean eaten = False;

    while (eaten == False)
    {
        randomNumber = rand() % 5 + 1;
        sleep(randomNumber);
        pickLeft(name);
        randomNumber = rand() % 7 + 2;
        sleep(randomNumber);
        pickRight(name, &eaten);
    }
    return NULL;
}

int main(int argc, char const *argv[])
{
    srand(time(NULL));
    pthread_t *proffesors;

    proffesors = malloc(5 * sizeof(pthread_t));

    for (unsigned long id = 0; id < 5; id++)
    {
        pthread_create(&proffesors[id], // the handle for it
                       NULL,            // its attributes
                       professor,       // the function it should run
                       (void *)id);     // args to that functionchop

        // create chopsticks
        chopStickArray[id].name = id;
        chopStickArray[id].owner = 6;
        chopStickArray[id].pickedUp = False;
        pthread_mutex_init(&(chopStickArray[id].lock), NULL);
    }
    for (unsigned long id = 0; id < 5; id++)
    {
        pthread_join(proffesors[id], NULL);
    }
    free(proffesors);
    return 0;
}