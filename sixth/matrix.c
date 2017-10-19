#include <ntsid.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <printf.h>

#define N 4
#define STATE_GENERATING 0
#define STATE_SUMMING 1
#define STATE_PRINTING 2

pthread_mutex_t mutex;
int state;

int a[N][N], b[N][N], c[N][N];

void *generator(void *args) {
    for(;;) {
        if(state == STATE_GENERATING) {
            pthread_mutex_lock(&mutex);
            srand(time(NULL)); //чтобы генерировать разные циферки при разных запусках
            for(int i = 0; i < N; ++i)
                for(int j = 0; j < N; ++j) {
                    a[i][j] = rand() % 5; //чтобы все числа были однозначными для красоты вывода и простоты проверки
                    b[i][j] = rand() % 5;
                }
            state = STATE_SUMMING;
            pthread_mutex_unlock(&mutex);
        }
        usleep(100000); //в микросекундах
    }
}

void *summator(void *args) {
    for(;;) {
        if(state == STATE_SUMMING) {
            pthread_mutex_lock(&mutex);
            for(int i = 0; i < N; ++i)
                for(int j = 0; j < N; ++j)
                    c[i][j] = a[i][j] + b[i][j];
            state = STATE_PRINTING;
            pthread_mutex_unlock(&mutex);
        }
        usleep(100000);
    }
}

void print(const int matrix[N][N]) {
    for(int i = 0; i < N; ++i) {
        for(int j = 0; j < N; ++j)
            printf("%d ", matrix[i][j]);
        printf("\n");
    }
}

void *printer(void *args) {
    for(;;) {
        if(state == STATE_PRINTING) {
            pthread_mutex_lock(&mutex);
            print(&a);
            printf("+\n");
            print(&b);
            printf("=\n");
            print(&c);
            printf("\n");
            pthread_mutex_unlock(&mutex);
            printf("Waiting 5 seconds..\n");
            sleep(5);
            state = STATE_GENERATING;
        }
        usleep(100000);
    }
}

int matrix_main() {
    pthread_mutex_init(&mutex, NULL);
    pthread_t gen_thread, sum_thread, print_thread;
    pthread_create(&gen_thread, NULL, &generator, NULL);
    pthread_create(&sum_thread, NULL, &summator, NULL);
    pthread_create(&print_thread, NULL, &printer, NULL);
    int status;
    scanf("%d", &status);
    while(status != 0)
        scanf("%d", &status);
    pthread_mutex_destroy(&mutex);
    return 0;
}