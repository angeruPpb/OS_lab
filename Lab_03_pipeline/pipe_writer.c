#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h> // Para exit()

int main() {
    int fd;
    const char *myfifo = "/tmp/myfifo"; // Cambiar char* a const char*

    /* create the FIFO (named pipe) */
    if (mkfifo(myfifo, 0666) == -1) {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }

    /* write "Hi" to the FIFO */
    fd = open(myfifo, O_WRONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    if (write(fd, "Hi", sizeof("Hi")) == -1) {
        perror("write");
        close(fd);
        exit(EXIT_FAILURE);
    }

    close(fd);

    /* remove the FIFO */
    if (unlink(myfifo) == -1) {
        perror("unlink");
        exit(EXIT_FAILURE);
    }

    return 0;
}