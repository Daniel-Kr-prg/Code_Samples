#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#include <pthread.h>

sem_t sem;
pthread_mutex_t mut;
pthread_mutex_t runningMut;

struct XandString
{
    int X;
    char * text;
    struct XandString * next;
};

struct XandString * head;
struct XandString * tail;

int threadsRunning = 1;

void * InputHandling(void * index)
{
    int threadIndex = *((int *) index);
    pthread_mutex_lock(&runningMut);
    while (threadsRunning)
    {
        pthread_mutex_unlock(&runningMut);
        sem_wait(&sem);
        pthread_mutex_lock(&mut);
        if (head != NULL)
        {
            struct XandString * xas = head;
            head = head->next;
            if (head == NULL)
                tail = NULL;
            pthread_mutex_unlock(&mut);

            flockfile(stdout);
            printf("Thread %d:", threadIndex);
            for (int i = 0; i < xas->X; i++)
            {
                printf(" %s", xas->text);
            }
            printf("\n");
            funlockfile(stdout);

            free(xas->text);
            free(xas);
        }
        else
        {
            pthread_mutex_unlock(&mut);
        }
        pthread_mutex_lock(&runningMut);
    }
    pthread_mutex_unlock(&runningMut);
    free(index);
    fprintf(stderr, "CONSUMENT FINISHED\n");
    pthread_exit(NULL);
}

void * InputReceiving()
{
    int * exitVal = (int *)malloc(sizeof(int));
    *exitVal = 0;
    int ret, x;
    char *text;
    while ((ret = scanf("%d %ms", &x, &text)) == 2)
    {
        if (x < 0)
        {
            free(text);
            *exitVal = 1;
            break;
        }
        struct XandString *xas = (struct XandString *)malloc(sizeof(struct XandString));
        xas->text = text;
        xas->X = x;
        xas->next = NULL;

        pthread_mutex_lock(&mut);
        if (tail == NULL)
        {
            head = xas;
            tail = xas;
        }
        else
        {
            tail->next = xas;
            tail = xas;
        }
        pthread_mutex_unlock(&mut);
        sem_post(&sem);
    }
    if (ret != EOF)
        *exitVal = 1;
    fprintf(stderr, "PRODUCENT FINISHED\n");
    pthread_exit(exitVal);
}

int main(int argc, char *argv[])
{
    int threadCount = 1;

    if (argc == 2)
    {
        int tc = atoi(argv[1]);
        if (tc == 0)
        {
            exit(1);
        }
        if (tc > sysconf(_SC_NPROCESSORS_ONLN))
        {
            exit(1);
        }
        threadCount = tc;
    }

    if (sem_init(&sem, 0, 0) == -1)
    {
        exit(1);
    }
    if (pthread_mutex_init(&mut, NULL) == -1)
    {
        sem_destroy(&sem);
        exit(1);
    }
    if (pthread_mutex_init(&runningMut, NULL) == -1)
    {
        sem_destroy(&sem);
        pthread_mutex_destroy(&mut);
        exit(1);
    }


    void * exitValue;

    head = (struct XandString *)malloc(sizeof(struct XandString));
    head = NULL;
    tail = (struct XandString *)malloc(sizeof(struct XandString));
    tail = NULL;

    pthread_t readThread;
    pthread_create(&readThread, 0, InputReceiving, NULL);

    // THREADS CREATE
    pthread_t threads[threadCount];
    for (int i = 0; i < threadCount; i++)
    {
        int * threadIndex = malloc(sizeof(*threadIndex)); 
        *threadIndex = i+1;
        pthread_create(&threads[i], 0, InputHandling, threadIndex);
    }
    // WHILE CYCLE




    pthread_join(readThread, &exitValue);
    int exitVal = *((int *)exitValue);

    fprintf(stderr, "PROD joined \n");

    pthread_mutex_lock(&runningMut);
    threadsRunning = 0;
    pthread_mutex_unlock(&runningMut);

    for (int i = 0; i < threadCount; i++)
    {
        sem_post(&sem);
    }
    fprintf(stderr, "sent posts\n");
    for (size_t i = 0; i < threadCount; i++)
    {
        pthread_join(threads[i], NULL);
    }
    
    fprintf(stderr, "ALL THREADS FINISHED\n");

    sem_destroy(&sem);
    pthread_mutex_destroy(&mut);

    while (head != NULL)
    {
        struct XandString * xas = head;
        head = xas->next;
        free(xas->text);
        free(xas);
    }

    free(head);
    free(tail);
    free(exitValue);
    exit(exitVal);
}