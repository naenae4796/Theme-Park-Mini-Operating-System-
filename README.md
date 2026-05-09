# Theme-Park-Mini-Operating-System-

Project Overview
This project is a user‑space MiniOS simulation modeled as an amusement park. Guest groups act as processes, rides act as shared resources, and the park has a fixed memory capacity. The simulation demonstrates core operating system concepts including process states, CPU scheduling, memory allocation, shared resource management, blocking, and synchronization. The system is implemented in C++ and advances through a simulated system clock.

# System Components

Guest
Represents a process or job with attributes such as process ID, arrival time, CPU burst time, memory requirement, priority, requested ride, and current state.

Scheduler
Selects the next READY process. Supports the following scheduling policies:
First Come First Served
Priority Scheduling (lower number means higher priority)
Round Robin with a configurable time quantum (default 2)

Memory Manager
Manages a fixed memory size of 1024 units using first‑fit contiguous allocation. Allocates memory before a process can enter READY or RUNNING and releases memory when a process terminates. If memory is unavailable, the process waits.

Ride (Shared Resource)
Represents a shared resource with capacity, duration, and a waiting line. Rides implemented include Carousel, RollerCoaster, and Ferris. If a ride is full or busy, the process blocks in the WAITING state.

Simulation Controller
Controls the system clock, state transitions, scheduling, memory allocation, resource usage, and logging.

# Scheduling Summary
The simulation supports FCFS, Priority Scheduling, and Round Robin. The scheduling policy is selected using command‑line arguments such as:
-policy fcfs
-policy priority
-policy rr (quantum 2)

# Memory Summary
Total memory size is 1024 units. Processes must have memory allocated before entering READY or RUNNING. First‑fit contiguous allocation is used.

# Shared Resource Summary
Rides act as shared resources with capacity limits and FCFS waiting lines. Processes block if a ride is full or busy.

# Jobs / Scenario
The simulation creates five guest processes with varying arrival times, CPU bursts, memory requirements, priorities, and ride selections. This satisfies the requirement for at least five jobs.

# Build and Run Instructions
Navigate to the Scheduler_And_Rides directory.
Use CMake to configure and build the project.
Run the executable with the desired scheduling policy.
On Windows, the executable may be located in a Debug or Release folder.

# Logs
The simulation logs process arrivals, memory allocation and release, scheduler selections, state changes, resource requests, blocking events, ride usage, preemption, and process termination.

# Summary
This simulation demonstrates core operating system concepts through a theme‑park model. It includes scheduling, memory management, synchronization, resource control, and detailed logging.
