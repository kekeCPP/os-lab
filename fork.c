#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    pid_t pid, pid2;
    unsigned i;
    unsigned niterations = 100;
    pid = fork();
    //pid == 0 if child
    if (pid == 0) {
        for (i = 0; i < niterations; ++i)
            printf("A = %d, ", i);
     } else {
        pid2 = fork();
        if(pid2 == 0){
            for (i = 0; i < niterations; ++i)
                printf("B = %d, ", i);
        } else {
            for (i = 0; i < niterations; ++i)
                printf("C = %d, ", i);
            printf("\n\nFirst childId: %d\n", pid);
            printf("Second childId: %d", pid2);
        }
    }
    printf("\n");
}