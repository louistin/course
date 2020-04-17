#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#define BUFFER_SIZE     4096
#define COPY_MODE       0644

static void oops(const char *s1, const char *s2) {
    fprintf(stderr, "Error: %s", s1);
    perror(s2);
    exit(1);
}

int main(int argc, char const *argv[])
{
    int in_fd;
    int out_fd;
    int n_chars;
    char buff[BUFFER_SIZE];

    if (argc != 3) {
        fprintf(stderr, "usage: %s source destination\n", argv[0]);
        exit(1);
    }

    if ((in_fd = open(argv[1], O_RDONLY)) == -1) {
        oops("Cannot open ", argv[1]);
    }

    if ((out_fd = creat(argv[2], COPY_MODE)) == -1) {
        oops("Cannot creat ", argv[2]);
    }

    while ((n_chars = read(in_fd, buff, sizeof(buff))) > 0) {
        if (write(out_fd, buff, n_chars) != n_chars) {
            oops("Write error to ", argv[2]);
        }
    }

    if (n_chars == -1) {
        oops("Read error from ", argv[1]);
    }

    if (close(in_fd) == -1 || close(out_fd) == -1) {
        oops("Error closing files", "");
    }

    return 0;
}
