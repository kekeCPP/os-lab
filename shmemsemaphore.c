#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h> /* For O_* constants */
#define SHMSIZE 128
#define SHM_R 0400
#define SHM_W 0200

int main(int argc, char **argv)
{
	//initialize randomizer
	srand(time(NULL));
	float randomNumber = 0.0;

	struct shm_struct {
		int buffer[10];
		int producer;
		int consumer;
	};
	volatile struct shm_struct *shmp = NULL;
	char *addr = NULL;
	pid_t pid = -1;
	int var1 = 0, var2 = 0, shmid = -1;
	struct shmid_ds *shm_buf;

    sem_t *sem_id1 = sem_open("semProducer", O_CREAT, O_RDWR, 0);
	sem_t *sem_id2 = sem_open("semConsumer", O_CREAT, O_RDWR, 1);

	/* allocate a chunk of shared memory */
	shmid = shmget(IPC_PRIVATE, SHMSIZE, IPC_CREAT | SHM_R | SHM_W);
	shmp = (struct shm_struct *) shmat(shmid, addr, 0);
	shmp->producer = 0;
	shmp->consumer = 0;

	pid = fork();
	if (pid != 0) {
		/* here's the parent, acting as producer */
		while (var1 < 100) {
			/* write to shmem */
			sem_wait(sem_id2);
            
            var1++;
			printf("Sending %d\n", var1); fflush(stdout);

			//dont let producer go over consumer
			// while(shmp->producer == shmp->consumer - 1 );
			// while(shmp->producer == 9 && shmp->consumer == 0);
			////////////////////////////////////////////////
			shmp->buffer[shmp->producer] = var1;

			//make producer loop (fake circle buffer)
			if(shmp->producer < 9) shmp->producer++;
			else shmp->producer = 0;
			////////////////////////////////////////////////

            sem_post(sem_id1);

			//gets a random number between 0.1 and 0.5
			randomNumber = ((rand() % 5) + 1.0) / 10.0;
			sleep(randomNumber);
		}
		shmdt(addr);
		shmctl(shmid, IPC_RMID, shm_buf);
		sem_close(sem_id1);
		sem_close(sem_id2);
	} else {
		/* here's the child, acting as consumer */
		while (var2 < 100) {
			/* read from shmem */
            sem_wait(sem_id1);
			//dont let consumer go before producer
			// while (shmp->consumer == shmp->producer);
			//////////////////////////////////////////

			var2 = shmp->buffer[shmp->consumer];

			//make consumer loop (fake circle)
			if(shmp->consumer < 9) shmp->consumer++;
			else shmp->consumer = 0;
			//////////////////////////////////////////

			printf("Received %d\n", var2); fflush(stdout);
            sem_post(sem_id2);

			randomNumber = ((rand() % 19) + 2.0) / 10.0;
			sleep(randomNumber);
		}
		shmdt(addr);
		shmctl(shmid, IPC_RMID, shm_buf);
		sem_close(sem_id1);
		sem_close(sem_id2);
		//wait(&status);
		sem_unlink("semProducer");
		sem_unlink("semConsumer");
	}
}