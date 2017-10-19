#include <sys/ipc.h>
#include <sys/shm.h>
#include <ntsid.h>
#include <stdlib.h>
#include <pthread.h>
#include <printf.h>

#define MAX_VERTEX_NUMBER 6

const int _graph[][2] = { //с почерком, потому что в proc_graph.c уже есть поле graph, а у меня это всё - один проект
        {0, 1},
        {0, 2},
        {0, 3},
        {1, 3},
        {1, 4},
        {2, 4},
        {3, 5}
};

int used[MAX_VERTEX_NUMBER];

const int graph_size = sizeof(_graph) / (2 * sizeof(int));

_Atomic int vertices; //атомик, потому что volatile не гарантирует атомарности инкремента

void *processor(void *args) {
    ++vertices;
    int v = *((int *) args); //у нас там хранится номер вершины
    printf("Hello there! I'm vertex number %d!\n", v);
    int children = 0; //запомним количество детей
    for(int i = 0; i < graph_size; ++i)
        if(_graph[i][0] == v && !used[_graph[i][1]])
            ++children;
    if(children == 0) //если ребер из нас в непосещенные вершины нет, то зачем жить
        return 0;
    int *child_args = malloc(children * sizeof(int)); //здесь будем хранить номера вершин детей
    pthread_t *child_threads = malloc(children * sizeof(pthread_t)); //а здесь - их треды
    int launched = 0;
    for(int i = 0; i < graph_size; ++i) {
        if(_graph[i][0] == v && !used[_graph[i][1]]) {
            used[_graph[i][1]] = 1;
            child_args[launched] = _graph[i][1]; //память уже выделена, можно просто приравнять
            if(pthread_create(&child_threads[launched], NULL, &processor, &child_args[launched++]) != 0) {
                perror("Could not create child thread worker!");
                break;
            }
        }
    }
    if(launched == 0) { //даже если children > 0, засчет установки значений в used (в цикле) children и launched могут быть не равны
        free(child_args);
        free(child_threads);
        return 0;
    }
    for(; launched > 0; --launched) { //проходимся только по кол-ву запущенных
        if(pthread_join(child_threads[launched - 1], NULL)) {
            perror("Could not join child thread worker!");
            break;
        }
    }
    free(child_args);
    free(child_threads);
    return 0;
}

int proc_thread_graph_main() {
    pthread_t worker;
    if(pthread_create(&worker, NULL, &processor, malloc(sizeof(int))) != 0) {
        perror("Could not create thread worker!");
        return 0;
    }
    pthread_join(worker, NULL);
    printf("Vertices amount: %d", vertices);
    return 0;
}