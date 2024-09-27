#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>

int BUFFER_SIZE = 1;

void read_file(char **filename){
    int fd = open(filename, O_RDONLY);

    if (fd == -1){
        err(1, "open() : File Not Found");
    }
    int r, w, n;
    char buffer[BUFFER_SIZE];
    while ((r = read(fd, buffer, BUFFER_SIZE)) != 0)
    {
        w = write(STDOUT_FILENO, buffer, r);
        n++;
        if (w == -1){
            err(1, "write() : Error in file read.");
        }
    }
    close(fd);
}

int main(int argc, char **argv)
{
    if (argc < 2){
        err(1, "Solver usage : ./solver <filename>");
    }
    read_file(argv[1]);
    return 0;
}