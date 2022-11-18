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

static void
init_matrix(void)
{
    int i, j;

    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
	        /* Simple initialization, which enables us to easy check
	         * the correct answer. Each element in c will have the same
	         * value as SIZE after the matmul operation.
	         */
	        a[i][j] = 1.0;
	        b[i][j] = 1.0;
        }
    }
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
    init_matrix();

    pthread_t* threads;
    struct threadArgs* args;
    threads = malloc(SIZE * sizeof(pthread_t));

    unsigned int i;
    for (i = 0; i < SIZE; i++) {
        args = malloc(sizeof(struct threadArgs));
        args->rowNumber = i;

        pthread_create(&(threads[i]),
        NULL,
        matmul_seq, // The threads will execute the matmul_seq function for each row
        (void*)args);
    }

    for (i = 0; i < SIZE; i++) {
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

    // matmul_seq();
    // print_matrix();
}
