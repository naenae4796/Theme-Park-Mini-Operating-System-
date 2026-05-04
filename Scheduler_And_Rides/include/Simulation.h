#pragma once
#include "Guest.h"
#include "MemoryManager.h"
#include "Ride.h"
#include "Scheduler.h"
#include <deque>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

// Tick-based simulation: 1 tick = 1 minute.
class Simulation {
 public:
  Simulation(std::vector<std::unique_ptr<Ride>> rides, std::vector<std::unique_ptr<Guest>> guests,
             SchedulingPolicy policy, std::ostream& log = std::cout);

  void setLogDelayMs(int delay_ms) { log_delay_ms_ = delay_ms; }
  void setStepMode(bool enabled) { step_mode_ = enabled; }
  void setRoundRobinQuantum(int quantum_ticks);
  void run(int max_ticks);

  int completed() const { return completed_; }
  double averageWait() const;
  double averageRideUtilization() const;

 private:
  void log(const std::string& msg);
  void admitNewArrivals(int tick);
  void updateWaitMetrics();
  void processRidePhase(int tick, std::vector<Guest*>& disembarked_all,
                        std::vector<Guest*>& admitted_all);
  void tryDispatchCpu(int tick);
  void cpuTick(int tick);
  void releaseMemoryFor(Guest& g, int tick);
  bool allGuestsFinished() const;

  std::vector<std::unique_ptr<Ride>> rides_;
  std::vector<std::unique_ptr<Guest>> guests_;
  std::deque<Guest*> ready_queue_;
  Guest* cpu_guest_ = nullptr;

  Scheduler scheduler_;
  std::ostream& log_;
  MemoryManager memory_{1024};
  int rr_quantum_ticks_ = 2;
  int log_delay_ms_ = 0;
  bool step_mode_ = false;

  int completed_ = 0;
  double wait_sum_ = 0.0;

  long long ride_busy_ticks_ = 0;
  int ticks_executed_ = 0;
};

std::vector<std::unique_ptr<Ride>> makeDemoRides();
