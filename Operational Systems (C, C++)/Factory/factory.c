#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

enum place {
    NUZKY, VRTACKA, OHYBACKA, SVARECKA, LAKOVNA, SROUBOVAK, FREZA,
    _PLACE_COUNT
};

const char* place_str[_PLACE_COUNT] = {
    [NUZKY] = "nuzky",
    [VRTACKA] = "vrtacka",
    [OHYBACKA] = "ohybacka",
    [SVARECKA] = "svarecka",
    [LAKOVNA] = "lakovna",
    [SROUBOVAK] = "sroubovak",
    [FREZA] = "freza",
};

float times_on_places[_PLACE_COUNT] = {
    100000,
    200000,
    150000,
    300000,
    400000,
    250000,
    500000
};
// float times_on_places[_PLACE_COUNT] = {
//     3,
//     2,
//     3.15,
//     3.3,
//     3.45,
//     3.25,
//     3.5};
enum product {
    A, B, C,
    _PRODUCT_COUNT
};

const char* product_str[_PRODUCT_COUNT] = {
    [A] = "A",
    [B] = "B",
    [C] = "C",
};

int find_string_in_array(const char** array, int length, char* what)
{
    for (int i = 0; i < length; i++)
        if (strcmp(array[i], what) == 0)
            return i;
    return -1;
}

int ready_places[_PLACE_COUNT];
int total_places[_PLACE_COUNT];

#define _PHASE_COUNT 6
int parts[_PRODUCT_COUNT][_PHASE_COUNT];

int places_in_processes[_PRODUCT_COUNT][_PHASE_COUNT] = {
    { NUZKY, VRTACKA, OHYBACKA, SVARECKA, VRTACKA, LAKOVNA },
    { VRTACKA, NUZKY, FREZA, VRTACKA, LAKOVNA, SROUBOVAK },
    { FREZA, VRTACKA, SROUBOVAK, VRTACKA, FREZA, LAKOVNA }
};

// WORKER STRUCTURE AND LIST
struct worker_data 
{
    pthread_t thread;
    int active;
    char * name;
};
struct worker
{
    int index;
    int place;
};

struct worker_data * work_data;
struct worker * workers;

int workers_count = 0;
int active_workers = 0;


int prods_in_progress = 0;

int workers_stuck = 0;

int finished = 0;
int finishAll = 0;
// WORKER

// COND VARS AND MUTEXES

    pthread_mutex_t wr_mut;
    // ready places
    pthread_mutex_t rp_mut;
    pthread_cond_t rp_cond;
    
    // parts
    pthread_mutex_t pa_mut;
    pthread_cond_t pa_cond;

// COND

void unsleepAll()
{
    pthread_mutex_lock(&pa_mut);
    workers_stuck = 0;
    pthread_cond_broadcast(&pa_cond);
    pthread_mutex_unlock(&pa_mut);

    pthread_mutex_lock(&rp_mut);
    pthread_cond_broadcast(&rp_cond);
    pthread_mutex_unlock(&rp_mut);
}

void * worker_trd(void * work)
{
    struct worker * info = (struct worker *) work;
    pthread_mutex_lock(&wr_mut);
    while (work_data[(*info).index].active)
    {
        pthread_mutex_unlock(&wr_mut);    

        int phase = -1;
        int prod = -1;
        pthread_mutex_lock(&pa_mut);
        while (phase == -1 && prod == -1)
        {
            for (int i = _PHASE_COUNT - 1; i >= 0 && phase == -1 && prod == -1; i--)
            {
                for (int j = 0; j < _PRODUCT_COUNT; j++)
                {
                    if (places_in_processes[j][i] == (*info).place && parts[j][i] > 0)
                    {
                        phase = i;
                        prod = j;
                        break;
                    }
                }
            }
            if (phase == -1 && prod == -1)
            {
                pthread_mutex_lock(&wr_mut);
                workers_stuck += 1;
                //fprintf(stderr, "%s : parts | stuck %i active %i\n", work_data[info->index].name, workers_stuck, active_workers);
                //fprintf(stderr, "Place: %i | workers stuck: %i from %i | parts in progress: %i\n",info->place, workers_stuck, active_workers, workers_in_progress);
                if (!finishAll)
                {
                    finishAll = finished && (workers_stuck >= active_workers || !prods_in_progress);
                }
                if (!work_data[(*info).index].active || finishAll)
                {
                    free(work_data[(*info).index].name);
                    active_workers -= 1;
                    pthread_mutex_unlock(&wr_mut);
                    pthread_mutex_unlock(&pa_mut);
                    free(info);
                    if (finishAll)
                        unsleepAll();
                    //fprintf(stderr, "ended on parts\n");
                    pthread_exit(NULL);
                }
                pthread_mutex_unlock(&wr_mut);
                pthread_cond_wait(&pa_cond, &pa_mut);
                pthread_mutex_lock(&wr_mut);
                if (finishAll)
                {
                    free(work_data[(*info).index].name);
                    active_workers -= 1;
                    pthread_mutex_unlock(&wr_mut);
                    pthread_mutex_unlock(&pa_mut);
                    free(info);
                    pthread_exit(NULL);
                }
                pthread_mutex_unlock(&wr_mut);
            }
        }
        //fprintf(stderr, "%s : phase %i prod %i\n", work_data[info->index].name, phase, prod);
        parts[prod][phase] -= 1;
        pthread_mutex_unlock(&pa_mut);

        pthread_mutex_lock(&rp_mut);
        while (ready_places[(*info).place] >= total_places[(*info).place])
        {
            //fprintf(stderr, "%s ready places? \n", work_data[info->index].name);
            pthread_cond_wait(&rp_cond, &rp_mut);
            if (!work_data[(*info).index].active || (finished && total_places[(*info).place] == 0))
            {
                //fprintf(stderr, "%s no places\n ", work_data[info->index].name);
                pthread_mutex_unlock(&rp_mut);
                pthread_mutex_lock(&wr_mut);
                active_workers -= 1;
                free(work_data[(*info).index].name);
                pthread_mutex_unlock(&wr_mut);
                free(info);
                unsleepAll();
                pthread_exit(NULL);
            }
        }
        fprintf(stderr, "%s found place\n", work_data[info->index].name);
        ready_places[(*info).place] += 1;
        pthread_mutex_unlock(&rp_mut);

        flockfile(stdout);
        printf("%s %s %i %s\n", work_data[(*info).index].name, place_str[(*info).place], phase + 1, product_str[prod]);
        funlockfile(stdout);
        usleep(times_on_places[(*info).place]);
        
        //fprintf(stderr, "slept\n");
        
        pthread_mutex_lock(&pa_mut);
        if (phase == 5)
        {
            prods_in_progress -= 1;
            flockfile(stdout);
            printf("done %s\n", product_str[prod]);
            funlockfile(stdout);
        }
        else
        {
            parts[prod][phase+1] += 1;
        }
        workers_stuck = 0;
        pthread_cond_broadcast(&pa_cond);
        pthread_mutex_unlock(&pa_mut);
        pthread_mutex_lock(&rp_mut);
        ready_places[(*info).place] -= 1;
        pthread_cond_broadcast(&rp_cond);
        pthread_mutex_unlock(&rp_mut);
        

        pthread_mutex_lock(&wr_mut);    
    }
    active_workers -= 1;
    free(work_data[(*info).index].name);
    pthread_mutex_unlock(&wr_mut);
    free(info);
    pthread_exit(NULL);
}

void createWorker(char * new_name, int place)
{
    // int newWorkerIndex = 0;
    // if (active_workers != workers_count)
    // {
    //     for (int i = 0; i < workers_count; i++)
    //     {
    //         if (!workers[i].active)
    //         {
    //             newWorkerIndex = i;
    //             break;
    //         }
    //     }
    // }
    // else
    // {
    //     //fprintf(stderr, "realloc\n");
    // }
    pthread_mutex_lock(&wr_mut);
    workers_count += 1;
    struct worker * worker = (struct worker *)malloc(sizeof(struct worker));

    work_data = (struct worker_data *)realloc(work_data, sizeof(struct worker_data) * (workers_count));

    work_data[workers_count-1].name = strdup(new_name);
    work_data[workers_count-1].active = 1;
    active_workers += 1;
    pthread_mutex_unlock(&wr_mut);

    worker->index = workers_count-1;
    worker->place = place;


    pthread_create(&work_data[workers_count - 1].thread, NULL, worker_trd, worker);
    //fprintf(stderr, "new worker: %s %i %i\n", workers[newWorkerIndex].name, workers[newWorkerIndex].place, workers[newWorkerIndex].active);
    
}

void clearWorker(char * name)
{
    int idx = -1;
    pthread_mutex_lock(&wr_mut);
    for (int i = 0; i < workers_count; i++)
        if (strcmp(name, work_data[i].name) == 0)
        {
            idx = i;
            break;
        }
    if (idx == -1)
        return;
    work_data[idx].active = 0;
    pthread_mutex_unlock(&wr_mut);
}
// void clearWorker(int idx)
// {
//     workers[idx].active = 0;
//     pthread_mutex_unlock(&wr_mut);
//     active_workers -= 1;
// }

int main(int argc, char** argv)
{
    if (pthread_mutex_init(&wr_mut, NULL) == -1)
    {
        exit(1);
    }
    if (pthread_mutex_init(&pa_mut, NULL) == -1)
    {
        pthread_mutex_destroy(&wr_mut);
        exit(1);
    }
    if (pthread_mutex_init(&rp_mut, NULL) == -1)
    {
        pthread_mutex_destroy(&wr_mut);
        pthread_mutex_destroy(&pa_mut);
        exit(1);
    }
    if (pthread_cond_init(&pa_cond, NULL) == -1)
    {
        pthread_mutex_destroy(&wr_mut);
        pthread_mutex_destroy(&pa_mut);
        pthread_mutex_destroy(&rp_mut);
        exit(1);
    }
    if (pthread_cond_init(&rp_cond, NULL) == -1)
    {
        pthread_mutex_destroy(&wr_mut);
        pthread_mutex_destroy(&pa_mut);
        pthread_mutex_destroy(&rp_mut);
        pthread_cond_destroy(&pa_cond);
        exit(1);
    }
    pthread_mutex_lock(&wr_mut);
    work_data = (struct worker_data *)malloc(0);
    pthread_mutex_unlock(&wr_mut);
    while (1) {
        char* line, * cmd, * arg1, * arg2, * arg3, * saveptr;
        int s = scanf(" %m[^\n]", &line);
        if (s == EOF || (strcmp(line, "ending") == 0))
            break;
        if (s == 0)
            continue;
        cmd = strtok_r(line, " ", &saveptr);
        arg1 = strtok_r(NULL, " ", &saveptr);
        arg2 = strtok_r(NULL, " ", &saveptr);
        arg3 = strtok_r(NULL, " ", &saveptr);

        if (strcmp(cmd, "start") == 0 && arg1 && arg2 && !arg3) {
            int place = find_string_in_array(
                place_str,
                _PLACE_COUNT,
                arg2
            );
            if (place < 0)
            {
                //fprintf(stderr, "ERROR: wrong place - %s\n", arg2);
            }
            else
                createWorker(arg1, place);
        }
        else if (strcmp(cmd, "make") == 0 && arg1 && !arg2) {
            int product = find_string_in_array(
                product_str,
                _PRODUCT_COUNT,
                arg1
            );
            
            if (product >= 0) 
            {
                pthread_mutex_lock(&pa_mut);
                parts[product][0]+=1;
                //fprintf(stderr, "new product: %s %i \n", product_str[product], parts[product][0]);
                prods_in_progress += 1;
                workers_stuck = 0;
                pthread_cond_broadcast(&pa_cond);
                pthread_mutex_unlock(&pa_mut);
            }
           
        }
        else if (strcmp(cmd, "end") == 0 && arg1 && !arg2) 
        {
            clearWorker(arg1);
            
            pthread_mutex_lock(&pa_mut);
            workers_stuck = 0;
            pthread_cond_broadcast(&pa_cond);

            pthread_mutex_unlock(&pa_mut);
        }
        else if (strcmp(cmd, "add") == 0 && arg1 && !arg2) {
            int place = find_string_in_array(
                place_str,
                _PLACE_COUNT,
                arg1
            );
            if (arg1 >= 0)
            {
                pthread_mutex_lock(&rp_mut);
                fprintf(stderr, "new place: %s\n", place_str[place]);
                total_places[place] += 1;
                pthread_cond_broadcast(&rp_cond);
                pthread_mutex_unlock(&rp_mut);
            }
        }
        else if (strcmp(cmd, "remove") == 0 && arg1 && !arg2) {
            int place = find_string_in_array(
                place_str,
                _PLACE_COUNT,
                arg1
            );
            pthread_mutex_lock(&rp_mut);
            total_places[place] -= 1;
            pthread_mutex_unlock(&rp_mut);
        }
        else {
            fprintf(stderr, "Invalid command: %s\n", line);
        }
        free(line);
    }

    //fprintf(stderr, "ending program\n");
    pthread_mutex_lock(&wr_mut);
    //fprintf(stderr, "finishing\n");
    finished = 1;
    pthread_mutex_unlock(&wr_mut);
    unsleepAll();


    for (int i = 0; i < workers_count; i++)
    {
        pthread_join(work_data[i].thread, NULL);
    }

    free(work_data);

    //fprintf(stderr, "finished\n");
    pthread_mutex_destroy(&wr_mut);
    pthread_mutex_destroy(&pa_mut);
    pthread_mutex_destroy(&rp_mut);
    pthread_cond_destroy(&pa_cond);
    pthread_cond_destroy(&rp_cond);
    exit(0);
}