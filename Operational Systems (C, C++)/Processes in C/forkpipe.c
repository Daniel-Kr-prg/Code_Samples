 #include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

int gen_doing = 1;

void sig_handler(int signum)
{
    gen_doing = 0;
}

int main(int argc, char *argv[])
{
    int pipefd[2];

    if (pipe(pipefd) == -1) 
    {
        exit(2);
    }

    pid_t cpid1, cpid2;
    cpid1 = fork();
    if (cpid1 == -1) 
    {
        exit(2);
    }

    if (cpid1 == 0) 
    {    /* GEN process */
        if (dup2(pipefd[1], 1) == -1)
            exit(2);
        if (close(pipefd[0]) == -1)
            exit(2);
        if (close(pipefd[1]) == -1)
            exit(2);

        signal(SIGTERM, sig_handler);

        while (gen_doing)
        {
            printf("%d %d\n", rand() % 4096, rand() % 4096);
            fflush(stdout);
            sleep(1);
        }
        fprintf(stderr, "GEN TERMINATED\n");

        _exit(0);

    } 
    else 
    {
        cpid2 = fork();
        if (cpid2 == -1)
        {
            exit(2);
        }

        if (cpid2 == 0) /* NSD */
        {
            if (dup2(pipefd[0], 0) == -1)
                exit(2);
            if (close(pipefd[0]) == -1)
                exit(2);
            if (close(pipefd[1]) == -1)
                exit(2);
            if (execl("nsd", "nsd", NULL) == -1)
                exit(2);
        }
        else /* MAIN process */
        {
            if (close(pipefd[0]) == -1)
                exit(2);
            if (close(pipefd[1]) == -1)
                exit(2);
            sleep(5);

            kill(cpid1, SIGTERM);

            int waitResult1, waitResult2;
            
            if (waitpid(cpid1, &waitResult1, 0) == -1)
                exit(2);
            if (waitpid(cpid2, &waitResult2, 0) == -1)
                exit(2);
            if (waitResult1 != 0 || waitResult2 != 0)
            {
                printf("ERROR\n");
                exit(1);
            }

            printf("OK\n");
            exit(0);
            
        }
    }
}