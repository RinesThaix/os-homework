#include <ntsid.h>
#include <unistd.h>
#include <printf.h>
#include <stdlib.h>
#include <errno.h>

//Матрица смежности
const int graph[][2] = {
        {0, 1},
        {0, 2},
        {0, 3},
        {1, 3},
        {1, 4},
        {2, 4},
        {3, 5}
};

//Для пайпа
int cache[2];

void dfs(int v, int size, pid_t initiator) {
    int next = 0;
    for(int i = 0; i < size; ++i) {
        next = i;
        if(graph[i][0] == v)
            if(fork() == 0) {
                dfs(graph[next][1], size, initiator);
                break;
            }
    }
    int vertices;
    read(cache[0], &vertices, sizeof(int));
    ++vertices;
    write(cache[1], &vertices, sizeof(int));
    printf("Process - %d | Vertices at the moment: %d\n", getpid(), vertices);
    //Ждем завершения выполнения всех наших детей
    int status, pid;
    while(pid = wait(&status)) {
        if(status == ECHILD || (pid == -1 && errno == ECHILD))
            break;
    }
    if(getpid() == initiator) {
        read(cache[0], &vertices, sizeof(int));
        printf("Vertices processed: %d\n", vertices);
    }
    exit(0);
}

int proc_graph_main() {
    int vertices = 0;
    pipe(cache);
    write(cache[1], &vertices, sizeof(int));
    dfs(0, sizeof(graph) / (2 * sizeof(int)), getpid());
    return 0;
}