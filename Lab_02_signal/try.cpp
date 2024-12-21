#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <cstdlib>

int main(int argc, char** argv) {   
    union sigval val = { .sival_int = atoi(argv[2]) };
    sigqueue((pid_t)atoi(argv[1]), SIGUSR1, val);
    return 0;
}