#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>
#include <stdlib.h>
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
			var1++;

			printf("Sending %d\n", var1); fflush(stdout);

			//dont let producer go over consumer
			while(shmp->producer == shmp->consumer - 1 );
			while(shmp->producer == 9 && shmp->consumer == 0);
			////////////////////////////////////////////////
			shmp->buffer[shmp->producer] = var1;

			//make producer loop (fake circle buffer)
			if(shmp->producer < 9) shmp->producer++;
			else shmp->producer = 0;
			////////////////////////////////////////////////

			//gets a random number between 0.1 and 0.5
			randomNumber = ((rand() % 5) + 1.0) / 10.0;
			sleep(randomNumber);
		}
		shmdt(addr);
		shmctl(shmid, IPC_RMID, shm_buf);
	} else {
		/* here's the child, acting as consumer */
		while (var2 < 100) {
			/* read from shmem */

			//dont let consumer go before producer
			while (shmp->consumer == shmp->producer);
			//////////////////////////////////////////

			var2 = shmp->buffer[shmp->consumer];

			//make consumer loop (fake circle)
			if(shmp->consumer < 9) shmp->consumer++;
			else shmp->consumer = 0;
			//////////////////////////////////////////

			printf("Received %d\n", var2); fflush(stdout);

			randomNumber = ((rand() % 19) + 2.0) / 10.0;
			sleep(randomNumber);
		}
		shmdt(addr);
		shmctl(shmid, IPC_RMID, shm_buf);
	}
}