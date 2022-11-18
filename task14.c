/***************************************************************************
 *
 * Sequential version of Matrix-Matrix multiplication
 *
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 1024

static double a[SIZE][SIZE];
static double b[SIZE][SIZE];
static double c[SIZE][SIZE];

struct threadArgs {
    unsigned int rowNumber;
};

// static void
void* init_matrix(void* params)
{
    struct threadArgs* initArgs = (struct threadArgs*) params;
    unsigned int row = initArgs->rowNumber;
    int j;

    // for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
	        /* Simple initialization, which enables us to easy check
	         * the correct answer. Each element in c will have the same
	         * value as SIZE after the matmul operation.
	         */
	        a[row][j] = 1.0;
	        b[row][j] = 1.0;
        }
    // }
    free(initArgs);
}


void* matmul_seq(void* params) {
    // Make the calculations for the given rownumber in args
    struct threadArgs* args = (struct threadArgs*) params;
    unsigned int row = args->rowNumber;
    int j, k;
    for (j = 0; j < SIZE; j++) {
            c[row][j] = 0.0;
            for (k = 0; k < SIZE; k++)
                c[row][j] = c[row][j] + a[row][k] * b[k][j];
        }
    free(args);
}

static void
print_matrix(void)
{
    int i, j;

    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++)
	        printf(" %7.2f", c[i][j]);
	    printf("\n");
    }
}

int
main(int argc, char **argv)
{
    // init_matrix();

    unsigned int i;

    pthread_t* initThreads;
    struct threadArgs* initArgs;
    initThreads = malloc(SIZE * sizeof(pthread_t));
    for (i = 0; i < SIZE; i++) {
        initArgs = malloc(sizeof(struct threadArgs));
        initArgs->rowNumber = i;

        pthread_create(&(initThreads[i]),
        NULL,
        init_matrix, // The threads will execute the init_matrix function
        (void*)initArgs);
    }

    pthread_t* threads;
    struct threadArgs* args;
    threads = malloc(SIZE * sizeof(pthread_t));

    for (i = 0; i < SIZE; i++) {
        args = malloc(sizeof(struct threadArgs));
        args->rowNumber = i;

        pthread_create(&(threads[i]),
        NULL,
        matmul_seq, // The threads will execute the matmul_seq function for each row
        (void*)args);
    }

    // pthread_join(initThread, NULL); // Wait initThread to terminate
    for (i = 0; i < SIZE; i++) {
        pthread_join(initThreads[i], NULL);
        pthread_join(threads[i], NULL); // Wait for all threads to terminate
    }


    // Make sure every value in matrix c is 1024
    unsigned int j;
    unsigned int pass = 1;
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            if (c[i][j] != 1024) {
                pass = 0;
            }
        }
    }
    if (pass = 1) { printf("IT PASSED"); }
    else { printf("IT FAILED"); }

    free(threads);
    free(initThreads);
    // matmul_seq();
    // print_matrix();
}
