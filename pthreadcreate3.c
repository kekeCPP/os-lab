#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct threadArgs {
	unsigned int id;
	unsigned int numThreads;
	unsigned int squareId;
};

void* child(void* params) {
	struct threadArgs *args = (struct threadArgs*) params;
	unsigned int childID = args->id;
	unsigned int numThreads = args->numThreads;
	args->squareId = childID * childID; // square the child id and store it in squareId
	printf("Greetings from child #%u of %u\n", childID, numThreads);
}

int main(int argc, char** argv) {
	pthread_t* children; // dynamic array of child threads
	struct threadArgs* args; // argument buffer
	unsigned int numThreads = 0;
	// get desired # of threads
	if (argc > 1)
		numThreads = atoi(argv[1]);
	children = malloc(numThreads * sizeof(pthread_t)); // allocate array of handles
	args = malloc(numThreads * sizeof(struct threadArgs)); // args vector
	for (unsigned int id = 0; id < numThreads; id++) {
		// create threads
		args[id].id = id;
		args[id].numThreads = numThreads;
		pthread_create(&(children[id]), // our handle for the child
			NULL, // attributes of the child
			child, // the function it should run
			(void*)&args[id]); // args to that function
	}
	printf("I am the parent (main) thread.\n");
	for (unsigned int id = 0; id < numThreads; id++) {
		pthread_join(children[id], NULL );
	}
	for (unsigned int id = 0; id < numThreads; id++) {
		printf("The squaredID for child number %d is %d\n", id, args[id].squareId); // Print all the squared id's after all children
																					// threads have terminated
	}
	free(args); // deallocate args vector
	free(children); // deallocate array
	return 0;
}
