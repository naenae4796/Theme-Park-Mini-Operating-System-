#include "ScenarioData.h"
#include "Simulation.h"
#include <cstdlib>
#include <iostream>
#include <string>

// Entry point: runs the park scenario with a selected policy, or all policies by default.

static const char* policyName(SchedulingPolicy p) {
  switch (p) {
    case SchedulingPolicy::FCFS:
      return "FCFS (FIFO READY queue)";
    case SchedulingPolicy::Priority:
      return "Priority (lower number = higher priority; ties: arrival, then pid)";
    case SchedulingPolicy::RoundRobin:
      return "Round Robin";
  }
  return "Unknown";
}

static bool parsePolicy(const std::string& value, SchedulingPolicy& out_policy) {
  if (value == "fcfs") {
    out_policy = SchedulingPolicy::FCFS;
    return true;
  }
  if (value == "priority") {
    out_policy = SchedulingPolicy::Priority;
    return true;
  }
  if (value == "rr" || value == "round-robin" || value == "roundrobin") {
    out_policy = SchedulingPolicy::RoundRobin;
    return true;
  }
  return false;
}

static void printUsage(const char* program) {
  std::cout << "Usage: " << program << " [--policy fcfs|priority|rr|all] [--quantum N]"
            << " [--demo|--tick-delay-ms N|--delay-ms N|--step|--fast]\n";
}

static void runCase(SchedulingPolicy p, int quantum_ticks, int event_delay_ms, int tick_delay_ms,
                    bool step_mode) {
  const char* title = policyName(p);
  std::cout << "\n######## " << title << " ########\n";
  Simulation sim(makeDemoRides(), makeBaseScenarioGuests(), p, std::cout);
  sim.setRoundRobinQuantum(quantum_ticks);
  sim.setLogDelayMs(event_delay_ms);
  sim.setTickDelayMs(tick_delay_ms);
  sim.setStepMode(step_mode);
  sim.run(120);
}

int main(int argc, char* argv[]) {
  int event_delay_ms = 0;
  int tick_delay_ms = 1000;
  int quantum_ticks = 2;
  bool step_mode = false;
  bool run_all = true;
  SchedulingPolicy selected_policy = SchedulingPolicy::FCFS;

  for (int i = 1; i < argc; ++i) {
    const std::string arg = argv[i];
    if (arg == "--step") {
      step_mode = true;
    } else if (arg == "--fast") {
      event_delay_ms = 0;
      tick_delay_ms = 0;
    } else if (arg == "--delay-ms" && i + 1 < argc) {
      event_delay_ms = std::atoi(argv[++i]);
      if (event_delay_ms < 0) {
        event_delay_ms = 0;
      }
    } else if (arg == "--tick-delay-ms" && i + 1 < argc) {
      tick_delay_ms = std::atoi(argv[++i]);
      if (tick_delay_ms < 0) {
        tick_delay_ms = 0;
      }
    } else if (arg == "--demo") {
      tick_delay_ms = 1000;
    } else if (arg == "--quantum" && i + 1 < argc) {
      quantum_ticks = std::atoi(argv[++i]);
      if (quantum_ticks <= 0) {
        std::cerr << "--quantum must be a positive integer.\n";
        return 1;
      }
    } else if (arg == "--policy" && i + 1 < argc) {
      const std::string value = argv[++i];
      if (value == "all") {
        run_all = true;
      } else if (parsePolicy(value, selected_policy)) {
        run_all = false;
      } else {
        std::cerr << "Unknown policy: " << value << "\n";
        printUsage(argv[0]);
        return 1;
      }
    } else if (arg == "--help" || arg == "-h") {
      printUsage(argv[0]);
      return 0;
    } else {
      std::cerr << "Unknown argument: " << arg << "\n";
      printUsage(argv[0]);
      return 1;
    }
  }

  if (run_all) {
    runCase(SchedulingPolicy::FCFS, quantum_ticks, event_delay_ms, tick_delay_ms, step_mode);
    runCase(SchedulingPolicy::Priority, quantum_ticks, event_delay_ms, tick_delay_ms, step_mode);
    runCase(SchedulingPolicy::RoundRobin, quantum_ticks, event_delay_ms, tick_delay_ms,
            step_mode);
  } else {
    runCase(selected_policy, quantum_ticks, event_delay_ms, tick_delay_ms, step_mode);
  }

  return 0;
}
