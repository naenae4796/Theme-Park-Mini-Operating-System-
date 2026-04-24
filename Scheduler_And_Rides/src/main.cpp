#include "ScenarioData.h"
#include "Simulation.h"
#include <iostream>

// Entry point: runs the same park scenario three times with FCFS, Priority, and Round Robin.

static void runCase(const char* title, SchedulingPolicy p) {
  std::cout << "\n######## " << title << " ########\n";
  Simulation sim(makeDemoRides(), makeBaseScenarioGuests(), p, std::cout);
  sim.run(120);
}

int main() {
  // 120 ticks = upper bound; simulation stops early when every guest is TERMINATED.
  runCase("FCFS (FIFO READY queue)", SchedulingPolicy::FCFS);
  runCase("Priority (lower number = higher priority; ties: arrival, then pid)",
          SchedulingPolicy::Priority);
  runCase("Round Robin (quantum = 2 ticks = 2 minutes)", SchedulingPolicy::RoundRobin);
  return 0;
}
