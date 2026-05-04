# OS_System - Person 2 Scheduler & Ride Behavior

This folder contains the **Person 2** portion of the Theme Park OS project. It was updated so it aligns with the untouched `Theme-Park-Mini-Operating-System--main` zip **without changing that base zip**.

## What this module demonstrates

- Process states: `NEW -> READY -> RUNNING -> WAITING -> TERMINATED`
- Scheduling policies:
  - FCFS
  - Priority Scheduling
  - Round Robin with a **2-minute quantum**
- Shared resource behavior through rides:
  - ride capacity
  - ride duration
  - busy/free states
  - FIFO waiting queues
  - release back to READY or TERMINATED after ride completion
- Fixed-size memory management:
  - 1024 total memory units
  - guests remain `NEW` until enough memory is available
  - memory is released when a guest reaches `TERMINATED`

## Compatibility with the base zip

This project now mirrors the base project's guest data model and scenario values:

- same guest fields as `GuestProcess`:
  - `pid`
  - `arrivalTime` equivalent -> `arrival_time`
  - `burstTime` equivalent -> `burst_time`
  - `remainingTime` equivalent -> `remaining_time`
  - `memoryRequirement` equivalent -> `memory_requirement`
  - `priority`
  - `rideRequest` equivalent -> `requested_ride_id`
  - `state`
- same scenario values as the original `ExampleScenario.cpp`

That means the logic in this folder can be explained and demoed against the untouched base project without altering teammates' files.

## File organization

- `include/Guest.h` - process model compatible with the base scenario fields
- `include/Scheduler.h` - FCFS, Priority, and RR selection logic
- `include/Ride.h` / `src/Ride.cpp` - shared ride resource and waiting queues
- `include/Simulation.h` / `src/Simulation.cpp` - state transitions, ready queue, CPU dispatch, ride coordination
- `include/ScenarioData.h` / `src/ScenarioData.cpp` - local copy of the base scenario values for standalone testing
- `src/main.cpp` - runs the three scheduling demos

## Build and run

### Linux / WSL / macOS

```bash
cmake -S . -B build
cmake --build build
./build/theme_park_os
```

For a slower classroom demo:

```bash
./build/theme_park_os --demo
```

For full manual stepping:

```bash
./build/theme_park_os --step
```

### Windows (Visual Studio generator)

```bash
cmake -S . -B build
cmake --build build --config Release
./build/Release/theme_park_os.exe
```

For a slower classroom demo:

```bash
./build/Release/theme_park_os.exe --demo
```

For full manual stepping:

```bash
./build/Release/theme_park_os.exe --step
```

## Visible behaviors in the output

The output shows the rubric items directly:

- arrivals into `READY`
- dispatch decisions per policy
- RR preemption at quantum 2
- ride blocking into `WAITING`
- ride admissions from queue
- ride completion and release
- termination
- memory allocation before `READY`
- memory release after `TERMINATED`

## Design notes

### Scheduling

- **FCFS**: picks the front of the READY queue
- **Priority**: picks the lowest priority number, then breaks ties by arrival time and pid
- **Round Robin**: picks FIFO from READY and preempts after 2 CPU ticks

### Resource / synchronization model

Rides act like shared resources:

- a guest tries to acquire a ride while RUNNING
- if the ride is busy or full, the guest blocks in `WAITING`
- waiting guests are held in a FIFO queue
- when a ride cycle finishes, guests are released and either:
  - return to `READY` if CPU work remains, or
  - move to `TERMINATED` if finished

## Architecture summary

```text
Guests (processes)
    |
    v
READY queue <-> Scheduler -> CPU dispatch
    |                       |
    |                       v
    |                  RUNNING guest
    |                       |
    |                 requests ride
    |                       v
    +----<---- WAITING queue / Ride resource ----> cycle complete
                               |
                               v
                        READY or TERMINATED
```

## Rubric mapping

- **Core Outcomes**: process model, states, scheduling, resource coordination all implemented
- **Correctness & Stability**: duplicate scheduler definition removed; simulation stops when all guests finish
- **Scheduling Implementation**: all 3 algorithms implemented and visible in logs
- **Resource & Synchronization Design**: rides block/release guests logically with FIFO waiting
- **Architecture & Code Organization**: modular headers and source files with separated responsibilities
- **Documentation**: this README explains build, mapping, and design decisions
