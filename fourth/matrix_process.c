#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

#define M 4
#define SHARED_M1 100
#define SHARED_M2 200
#define SHARED_M3 300
#define SHARED_ST 400
#define STATE_GENERATING 0
#define STATE_SUMMING 1
#define STATE_PRINTING 2
#define STATE_CLEARING 3

int init_memory(int key, size_t size) {
    int mem = shmget(key, size, IPC_CREAT | 0666);
    if(mem == -1) {
        perror("Could not allocate shared memory");
        exit(0);
    }
//    printf("Attached shmemory with id %d\n", mem);
    return mem;
}

int init_matrix(int key, int **matrix) {
    int mem = init_memory(key, M * M * sizeof(int));
    *matrix = (int *) shmat(mem, 0, 0);
    return mem;
}

int init_int(int key, int **integer) {
    int mem = init_memory(key, sizeof(int));
    *integer = (int *) shmat(mem, 0, 0);
    return mem;
}

//без этого совсем плохо
void free_memory(const int mem_id) {
    shmctl(mem_id, IPC_RMID, NULL);
//    printf("Detached shmemory with id %d\n", mem_id);
}

void generate(int *a, int *b, int *state) {
    while(1) {
        if(*state == STATE_GENERATING) {
            srand(time(NULL));
            for(int i = 0; i < M * M; ++i) {
                a[i] = rand() % 5;
                b[i] = rand() % 5;
            }
//            printf("GENERATED\n");
            *state = STATE_SUMMING;
        }else if(*state == STATE_CLEARING)
            return;
        usleep(100000);
    }
}

void summarize(const int *a, const int *b, int *c, int *state) {
    while(1) {
        if(*state == STATE_SUMMING) {
            for(int i = 0; i < M * M; ++i)
                c[i] = a[i] + b[i];
//            printf("SUMMARIZED\n");
            *state = STATE_PRINTING;
        }else if(*state == STATE_CLEARING)
            return;
        usleep(100000);
    }
}

void printm(const int *matrix) {
    int i = 0;
    for(int j = 0; j < M; ++j) {
        for(int k = 0; k < M; ++i, ++k)
            printf("%d ", matrix[i]);
        printf("\n");
    }
}

void do_print(const int *a, const int *b, const int *c, int *state) {
    while(1) {
        if(*state == STATE_PRINTING) {
            printm(a);
            printf("+\n");
            printm(b);
            printf("=\n");
            printm(c);
            printf("\n");
//            printf("Waiting 5 seconds..\n");
//            sleep(5);
//            printf("PRINTED\n");
            *state = STATE_CLEARING;
        }else if(*state == STATE_CLEARING)
            return;
        usleep(100000);
    }
}

int matrix_process_main() {
    int *a, *b, *c, *state;
    int aid = init_matrix(SHARED_M1, &a);
    int bid = init_matrix(SHARED_M2, &b);
    int cid = init_matrix(SHARED_M3, &c);
    int sid = init_int(SHARED_ST, &state);

    *state = STATE_GENERATING;

    //первый процесс генерирует, второй - считает, третий - печатает
    pid_t gen = fork();
    if(gen == 0)
        generate(a, b, state);
    else {
        pid_t sum = fork();
        if(sum == 0)
            summarize(a, b, c, state);
        else {
            pid_t print = fork();
            if(print == 0)
                do_print(a, b, c, state);
        }
    }

    while(*state != STATE_CLEARING);

    free_memory(aid);
    free_memory(bid);
    free_memory(cid);
    free_memory(sid);

//    printf("Cleared memory!\n");

    return 0;
}