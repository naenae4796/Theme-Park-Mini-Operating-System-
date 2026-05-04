#include "ScenarioData.h"
#include "Simulation.h"
#include <cstdlib>
#include <iostream>
#include <string>

// Entry point: runs the same park scenario three times with FCFS, Priority, and Round Robin.

static void runCase(const char* title, SchedulingPolicy p, int delay_ms, bool step_mode) {
  std::cout << "\n######## " << title << " ########\n";
  Simulation sim(makeDemoRides(), makeBaseScenarioGuests(), p, std::cout);
  sim.setLogDelayMs(delay_ms);
  sim.setStepMode(step_mode);
  sim.run(120);
}

int main(int argc, char* argv[]) {
  int delay_ms = 0;
  bool step_mode = false;

  for (int i = 1; i < argc; ++i) {
    const std::string arg = argv[i];
    if (arg == "--step") {
      step_mode = true;
    } else if (arg == "--delay-ms" && i + 1 < argc) {
      delay_ms = std::atoi(argv[++i]);
      if (delay_ms < 0) {
        delay_ms = 0;
      }
    } else if (arg == "--demo") {
      delay_ms = 450;
    }
  }

  // 120 ticks = upper bound; simulation stops early when every guest is TERMINATED.
  runCase("FCFS (FIFO READY queue)", SchedulingPolicy::FCFS, delay_ms, step_mode);
  runCase("Priority (lower number = higher priority; ties: arrival, then pid)",
          SchedulingPolicy::Priority, delay_ms, step_mode);
  runCase("Round Robin (quantum = 2 ticks = 2 minutes)", SchedulingPolicy::RoundRobin,
          delay_ms, step_mode);
  return 0;
}
