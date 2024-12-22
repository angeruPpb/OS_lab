/*****************************************************************************
 Excerpt from "Linux Programmer's Guide - Chapter 6"
 (C)opyright 1994-1995, Scott Burkett
 ***************************************************************************** 
 MODULE: pipe.c
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#define MAX_LINES 100
#define MAX_LINE_LENGTH 256

void read_lines_from_file(const char *filename, char lines[MAX_LINES][MAX_LINE_LENGTH], int *num_lines);

int main(void)
{
    int fd[2], nbytes;
    pid_t childpid;
    char readbuffer[80];
    char lines[MAX_LINES][MAX_LINE_LENGTH];
    int num_lines;

    // Leer las líneas del archivo
    

    pipe(fd);

    if ((childpid = fork()) == -1) {
        perror("fork");
        exit(1);
    }

    printf("Main program:"); 
    getchar();

    if (childpid == 0) {
        printf("Child program:"); 
        getchar();
        /* Child process closes up input side of pipe */
        close(fd[0]);
        read_lines_from_file("file_01.txt", lines, &num_lines);
        /* Send lines through the output side of pipe */
        for (int i = 0; i < num_lines; i++) {
            write(fd[1], lines[i], strlen(lines[i]) + 1);
        }
        exit(0);
    } else {
        printf("Parent program:"); 
        getchar();
        /* Parent process closes up output side of pipe */
        close(fd[1]);

        /* Read in strings from the pipe */
        while ((nbytes = read(fd[0], readbuffer, sizeof(readbuffer))) > 0) {
            printf("Received string: %s\n", readbuffer);
        }
    }

    return 0;
}

void read_lines_from_file(const char *filename, char lines[MAX_LINES][MAX_LINE_LENGTH], int *num_lines) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    *num_lines = 0;
    while (fgets(lines[*num_lines], MAX_LINE_LENGTH, file) != NULL) {
        // Remove newline character if present
        lines[*num_lines][strcspn(lines[*num_lines], "\n")] = '\0';
        (*num_lines)++;
        if (*num_lines >= MAX_LINES) {
            break;
        }
    }

    fclose(file);
}