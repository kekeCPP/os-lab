/***************************************************************************
 *
 * Sequential version of Matrix-Matrix multiplication
 *
 ***************************************************************************/





/////////////////////////////////////////////////////////////////////////
// THE PARALLEL VERSION OF THIS PROGRAM CAN BE FOUND IN matmulseqpar.c //
/////////////////////////////////////////////////////////////////////////

// T_sequential = 8.546s
// T_parallel = 1.059s
// Speedup = 8.070 (My cpu has 8 cores so this makes sense)
// T_parallel_init = 0.874s (It is slightly faster when init_matrix() also is parallel because then we can
//                          create multiple rows in the matrix at the same time.)

// T_parallel_granularity = 1.178s
// Speedup = 8.546 / 1.178 = 7.255 (Slightly slower Speedup when lowering the number of threads because less work
//                                 can be done simoltaniously. However more work is done per single thread.)




#include <stdio.h>
#include <stdlib.h>

#define SIZE 1024

static double a[SIZE][SIZE];
static double b[SIZE][SIZE];
static double c[SIZE][SIZE];

static void
init_matrix(void)
{
    int i, j;

    for (i = 0; i < SIZE; i++)
        for (j = 0; j < SIZE; j++) {
	        /* Simple initialization, which enables us to easy check
	         * the correct answer. Each element in c will have the same
	         * value as SIZE after the matmul operation.
	         */
	        a[i][j] = 1.0;
	        b[i][j] = 1.0;
        }
}

static void
matmul_seq()
{
    int i, j, k;

    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            c[i][j] = 0.0;
            for (k = 0; k < SIZE; k++)
                c[i][j] = c[i][j] + a[i][k] * b[k][j];
        }
    }
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
    matmul_seq();
    //print_matrix();
}
