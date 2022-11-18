#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

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
};
struct chopStick chopStickArray[5];

void putDown(unsigned long name)
{

    chopStickArray[name].pickedUp = False;
    chopStickArray[name].owner = 6;

    int tempName = (name + 1) % 5;
    chopStickArray[tempName].pickedUp = False;
    chopStickArray[tempName].owner = 6;
    printf("Proffesor %lu eating -> put down chopsticks\n", name);
}

void pickLeft(unsigned long name)
{
    while (chopStickArray[name].pickedUp != False);
    chopStickArray[name].pickedUp = True;
    chopStickArray[name].owner = name;
    printf("Proffesor %lu thinking -> got left chopstick\n", name);
}

void pickRight(unsigned long name, boolean *eaten)
{
    int tempName = (name + 1) % 5;
    int randomNumber = 0;
    boolean droppedLeft = False;

    while (chopStickArray[tempName].pickedUp != False && droppedLeft == False){
        chopStickArray[name].pickedUp = False;
        chopStickArray[name].owner = 6;
        droppedLeft = True;
    };
    if(droppedLeft == False){
        chopStickArray[tempName].pickedUp = True;
        chopStickArray[tempName].owner = name;
        printf("Proffesor %lu thinking -> got right chopstick -> eating\n", name);

        randomNumber = rand() % 6 + 5;
        *eaten = True;
        putDown(name);
    }
    else{
        printf("Proffesor %lu Dropped right chopstick, resetting\n", name);
    }
}

void *professor(void *buf)
{
    unsigned long name = (unsigned long)buf;
    int randomNumber = 0;
    boolean eaten = False;

    while(eaten == False){
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
    }
    for (unsigned long id = 0; id < 5; id++)
    {
        pthread_join(proffesors[id], NULL);
    }
    free(proffesors);
    return 0;
}