#include <printf.h>
#include <memory.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

void copy(const char *from, const char *to) {
    int from_open = open(from, O_RDONLY); //хотим только читать
    creat(to, S_IRWXO | S_IRWXU | S_IRWXG); //создаем выходной файл
    int to_open = open(to, O_WRONLY | O_TRUNC); //хотим сюда писать
    if(from_open < 0 || to_open < 0) {
        perror("Something went really wrong");
        return;
    }
    int byte, bufsize = 1 << 7;
    char *buf = (char*) malloc(bufsize);
    while(byte = read(from_open, buf, bufsize))//пока остались байты
        write(to_open, buf, bufsize);
    free(buf); //освобождаем всё занятое
    if(close(from_open) < 0 || close(to_open) < 0)
        perror("Couldn't close streams of input/output file");
}

int is_directory(const char *name) {
    return name[strlen(name) - 1] == '/';
}

int cp_main(int args_amount, char *args[]) {
    if(args_amount < 3) {
        printf("Use 'cp file1 file2' to copy first file to second one\n");
        printf("Use 'cp file1 file2/' to copy first file inside second one (file2 is a directory)\n");
        return 0;
    }
    copy(args[1], is_directory(args[2]) ? strcat(args[2], args[1]) : args[2]);
    return 0;
}