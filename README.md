# Theme-Park-Mini-Operating-System-

Project Overview
This project is a user‑space MiniOS simulation modeled as an amusement park. Guest groups act as processes, rides act as shared resources, and the park has a fixed memory capacity. The simulation demonstrates core operating system concepts including process states, CPU scheduling, memory allocation, shared resource management, blocking, and synchronization. The system is implemented in C++ and advances through a simulated system clock.

System Components

Guest
Represents a process or job. Each guest has:
Process ID
Arrival time
CPU burst time
Remaining CPU time
Memory requirement
Priority
Requested ride
Current state

Scheduler
Selects the next READY process. Supports the following scheduling policies:
First Come First Served
Priority Scheduling (lower number means higher priority)
Round Robin with a configurable time quantum (default 2)

Memory Manager
Manages a fixed memory size of 1024 units using first‑fit contiguous allocation.
Allocates memory before a process can enter READY or RUNNING.
Releases memory when a process terminates.
If memory is unavailable, the process waits.

Ride (Shared Resource)
Represents a shared resource with capacity, duration, and a waiting line.
Rides implemented include Carousel, RollerCoaster, and Ferris.
Each ride has a busy or free state and an FCFS waiting line.
If a ride is full or busy, the process blocks in the WAITING state.

Simulation Controller
Controls the system clock, state transitions, scheduling, memory allocation, resource usage, and logging.

Architecture (Text Description)

Guest processes enter the Memory Manager, which allocates fixed memory blocks.
Once memory is allocated, processes enter the READY queue.
The Scheduler selects a process from READY and assigns it to the CPU as the RUNNING process.
If the process requests a ride, it interacts with the Ride Resource.
If the ride is busy or full, the process moves to the WAITING queue.
When the ride becomes available, the process enters the RIDING state.
After riding, the process either returns to READY or TERMINATES.
When terminated, memory is released back to the Memory Manager.

Process States

NEW: Process has not yet entered the ready queue.
READY: Process has memory and is waiting for CPU scheduling.
RUNNING: Process is currently executing.
WAITING: Process is blocked waiting for a ride or resource.
RIDING: Process has acquired a ride resource.
TERMINATED: Process has completed and released memory.

Scheduling

The simulation supports three scheduling policies:
FCFS: First Come First Served
Priority Scheduling: Lower priority number means higher priority
Round Robin: Uses a configurable time quantum (default 2)

Scheduling can be selected using command line arguments such as:
--policy fcfs
--policy priority
--policy rr --quantum 2

Memory Management

Total memory size is 1024 units.
Each process has a memory requirement.
Memory must be allocated before the process can enter READY or RUNNING.
The memory manager uses first‑fit contiguous allocation.
If no block is large enough, the process waits.
Memory is released when the process terminates.

Shared Resources and Synchronization

Rides act as shared resources.
Each ride has capacity, duration, and a busy state.
If a ride is full or busy, the process blocks.
Synchronization is enforced using the ride’s busy state and waiting line.
This prevents multiple processes from using the same ride simultaneously.

Jobs / Scenario

The simulation creates five guest processes:

PID 0, Arrival 0, Burst 4, Memory 200, Priority 2, Ride Carousel
PID 1, Arrival 1, Burst 3, Memory 128, Priority 1, Ride RollerCoaster
PID 2, Arrival 2, Burst 6, Memory 256, Priority 3, Ride Carousel
PID 3, Arrival 3, Burst 2, Memory 128, Priority 2, Ride RollerCoaster
PID 4, Arrival 4, Burst 5, Memory 300, Priority 0, Ride Carousel

This satisfies the requirement for at least five jobs.

Build and Run Instructions

Navigate to the Scheduler_And_Rides directory.
Use CMake to configure and build the project.
Run the executable with the desired scheduling policy.
On Windows, the executable may be located in a Debug or Release folder.

Observable Logs

The simulation logs the following events:
Process arrivals
Memory allocation and release
Scheduler selections
RUNNING state changes
Resource requests
Blocking and waiting events
Ride and resource release
Round Robin preemption
Process termination
Summary statistics

These logs provide evidence that scheduling, memory management, resource contention, blocking, and state transitions are functioning correctly.

Summary

This simulation demonstrates core operating system concepts through a theme‑park model. It includes scheduling, memory management, synchronization, resource control, and detailed logging. The system meets the requirements outlined in the Final Project rubric.
