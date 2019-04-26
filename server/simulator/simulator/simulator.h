#ifndef SIMULATOR_H
#define SIMULATOR_H

#define TIMEOUT_SEC 1
#define BUFFER_SIZE 100
#define FRAME_RATE_NSEC 200000

struct process {
    int pid;
    int input_fd;
    int output_fd;
};

#endif