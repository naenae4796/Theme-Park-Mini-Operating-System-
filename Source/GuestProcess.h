#ifndef GUEST_PROCESS_H
#define GUEST_PROCESS_H

enum ProcessState { NEW, READY, RUNNING, WAITING, TERMINATED };

struct GuestProcess {
    int pid;
    int arrivalTime;
    int burstTime;
    int remainingTime;
    int memoryRequirement;
    int priority;
    int rideRequest;
    ProcessState state;
};

#endif
