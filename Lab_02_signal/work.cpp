#include <iostream>
#include <csignal>
#include <unistd.h>
#include <cstdlib>
using namespace std;

int main(int argc, char* argv[]) {
    pid_t pid = stoi(argv[1]);
    int signal_code = stoi(argv[2]);
    
    if (kill(pid, signal_code) == 0) {
        cout << "Signal " << signal_code << " sent successfully to process " << pid << endl;
    } else {
        perror("Error sending signal");
    }

    return 0;
}