// Simulation engine: fixed-order tick loop (admit -> ride timers/queues -> wait
// accounting -> CPU dispatch -> CPU/ride request step). CPU time and RR quantum
// apply only while a guest holds the CPU and is not riding; boarding releases the CPU.
//
// Each outer-loop tick: arrivals become READY, rides advance (busy timer, disembark,
// admit from wait queues), READY/WAITING guests accrue wait time, then one CPU guest
// may run and burn burst / request a ride / get preempted (RR quantum is configurable).
#include "Simulation.h"
#include "ScenarioData.h"
#include <algorithm>
#include <chrono>
#include <iostream>
#include <sstream>
#include <thread>

Simulation::Simulation(std::vector<std::unique_ptr<Ride>> rides,
                       std::vector<std::unique_ptr<Guest>> guests, SchedulingPolicy policy,
                       std::ostream& log)
    : rides_(std::move(rides)), guests_(std::move(guests)), scheduler_(policy), log_(log) {}

void Simulation::setRoundRobinQuantum(int quantum_ticks) {
  rr_quantum_ticks_ = std::max(1, quantum_ticks);
}

void Simulation::log(const std::string& msg) {
  log_ << msg << '\n';
  if (step_mode_) {
    log_ << "Press Enter for next event...";
    log_.flush();
    std::cin.get();
  } else if (log_delay_ms_ > 0) {
    std::this_thread::sleep_for(std::chrono::milliseconds(log_delay_ms_));
  }
}

// Mean total wait ticks across guests that reached TERMINATED (for summary stats).
double Simulation::averageWait() const {
  if (completed_ == 0) {
    return 0.0;
  }
  return wait_sum_ / static_cast<double>(completed_);
}

// Fraction of (ride × tick) slots where at least one ride was busy (rough utilization).
double Simulation::averageRideUtilization() const {
  const int n = static_cast<int>(rides_.size());
  if (n == 0 || ticks_executed_ == 0) {
    return 0.0;
  }
  return static_cast<double>(ride_busy_ticks_) / static_cast<double>(n * ticks_executed_);
}

// Guests whose arrival time has passed first request memory. Only guests with
// allocated memory may move from NEW to READY.
void Simulation::admitNewArrivals(int tick) {
  std::vector<Guest*> batch;
  for (auto& gptr : guests_) {
    Guest& g = *gptr;
    if (g.state == GuestState::NEW && tick >= g.arrival_time) {
      batch.push_back(&g);
    }
  }
  std::sort(batch.begin(), batch.end(), [](Guest* a, Guest* b) {
    if (a->arrival_time != b->arrival_time) {
      return a->arrival_time < b->arrival_time;
    }
    return a->pid < b->pid;
  });

  for (Guest* gp : batch) {
    Guest& g = *gp;
    if (!g.memory_allocated) {
      if (!memory_.allocate(g.memory_requirement)) {
        if (!g.memory_block_logged) {
          std::ostringstream os;
          os << "[t=" << tick << "] MEM_BLOCK pid=" << g.pid << " need="
             << g.memory_requirement << " available=" << memory_.available()
             << "/" << memory_.total() << " -> stays NEW";
          log(os.str());
          g.memory_block_logged = true;
        }
        continue;
      }

      g.memory_allocated = true;
      g.memory_start = memory_.lastAllocationStart();
      std::ostringstream mem;
      mem << "[t=" << tick << "] MEM_ALLOC pid=" << g.pid << " +"
          << g.memory_requirement << " used=" << memory_.used() << "/"
          << memory_.total() << " start=" << g.memory_start << " strategy=first-fit";
      log(mem.str());
    }

    g.state = GuestState::READY;
    if (g.first_ready_tick < 0) {
      g.first_ready_tick = tick;
    }
    ready_queue_.push_back(&g);
    std::ostringstream os;
    os << "[t=" << tick << "] ARRIVE pid=" << g.pid << " -> READY (burst=" << g.remaining_time
       << " pri=" << g.priority << " mem=" << g.memory_requirement << " ride=" << g.requested_ride_id << ")";
    log(os.str());
  }
}

// Per-tick wait accounting: READY and blocked WAITING guests accumulate one wait tick.
void Simulation::updateWaitMetrics() {
  for (auto& gptr : guests_) {
    Guest& g = *gptr;
    if (g.state == GuestState::READY || g.state == GuestState::WAITING) {
      g.total_wait_ticks += 1;
    }
  }
}

// Advances every ride for this tick: disembarking guests go back to READY or TERMINATED;
// admissions from the ride's wait queue are logged. Updates ride utilization counter.
void Simulation::processRidePhase(int tick, std::vector<Guest*>& disembarked_all,
                                    std::vector<Guest*>& admitted_all) {
  disembarked_all.clear();
  admitted_all.clear();

  for (auto& r : rides_) {
    std::vector<Guest*> out_d;
    std::vector<Guest*> out_a;
    bool ride_operating = false;
    r->tick(out_d, out_a, ride_operating);

    if (ride_operating) {
      ++ride_busy_ticks_;
    }

    for (Guest* g : out_d) {
      if (!g) {
        continue;
      }
      disembarked_all.push_back(g);
      if (g->requested_ride_id == r->id()) {
        g->visit_completed = true;
      }

      if (g->remaining_time > 0) {
        g->state = GuestState::READY;
        ready_queue_.push_back(g);
        log("[t=" + std::to_string(tick) + "] RIDE_COMPLETE pid=" + std::to_string(g->pid) +
            " ride=" + r->name() + " -> READY (cpu_left=" + std::to_string(g->remaining_time) +
            ")");
      } else if (g->visit_completed || g->requested_ride_id < 0) {
        g->state = GuestState::TERMINATED;
        wait_sum_ += g->total_wait_ticks;
        ++completed_;
        releaseMemoryFor(*g, tick);
        log("[t=" + std::to_string(tick) + "] RIDE_COMPLETE pid=" + std::to_string(g->pid) +
            " ride=" + r->name() + " -> TERMINATED (cpu done, visit done)");
      } else {
        g->state = GuestState::READY;
        ready_queue_.push_back(g);
        log("[t=" + std::to_string(tick) + "] RIDE_COMPLETE pid=" + std::to_string(g->pid) +
            " ride=" + r->name() + " -> READY (finish cpu then revisit)");
      }
    }

    for (Guest* g : out_a) {
      if (!g) {
        continue;
      }
      admitted_all.push_back(g);
      log("[t=" + std::to_string(tick) + "] RIDE_ADMIT_WAIT pid=" + std::to_string(g->pid) +
          " ride=" + r->name() + " WAITING -> RIDING");
    }
  }
}

// If the CPU is idle, pick the next READY guest per policy; skip guests still "on ride"
// (they stay queued until not riding).
void Simulation::tryDispatchCpu(int tick) {
  if (cpu_guest_ != nullptr) {
    return;
  }

  Guest* next = scheduler_.pickNext(ready_queue_);
  if (!next) {
    return;
  }

  if (next->riding) {
    ready_queue_.push_back(next);
    return;
  }

  next->state = GuestState::RUNNING;
  next->rr_cpu_used_this_slice = 0;
  cpu_guest_ = next;

  std::ostringstream os;
  os << "[t=" << tick << "] DISPATCH pid=" << next->pid << " policy=";
  os << (scheduler_.policy() == SchedulingPolicy::FCFS
             ? "FCFS"
             : (scheduler_.policy() == SchedulingPolicy::Priority ? "PRIORITY" : "RR"));
  log(os.str());
}

// One tick of work for whoever holds the CPU: optionally board a requested ride (may block
// to WAITING), else consume one unit of CPU burst; RR may preempt after the configured quantum.
void Simulation::cpuTick(int tick) {
  if (!cpu_guest_) {
    return;
  }

  Guest& g = *cpu_guest_;

  if (g.riding) {
    cpu_guest_ = nullptr;
    return;
  }

  Ride* ride = nullptr;
  for (auto& r : rides_) {
    if (r->id() == g.requested_ride_id) {
      ride = r.get();
      break;
    }
  }

  const bool need_visit = (g.requested_ride_id >= 0 && !g.visit_completed);

  if (need_visit) {
    if (!ride) {
      g.visit_completed = true;
    } else {
      const bool boarded = ride->tryBoard(g);
      if (!boarded) {
        std::ostringstream os;
        os << "[t=" << tick << "] RIDE_BLOCK pid=" << g.pid << " ride=" << ride->name()
           << " -> WAITING (queue_len=" << ride->waiting_queue().size() << ")";
        log(os.str());
        cpu_guest_ = nullptr;
        return;
      }
      std::ostringstream os;
      os << "[t=" << tick << "] RIDE_BOARD pid=" << g.pid << " ride=" << ride->name()
         << " RUNNING -> RIDING";
      log(os.str());
      cpu_guest_ = nullptr;
      return;
    }
  }

  const int cpu_before = g.remaining_time;
  if (g.remaining_time > 0) {
    g.remaining_time -= 1;
  }

  if (scheduler_.policy() == SchedulingPolicy::RoundRobin && cpu_before > 0) {
    g.rr_cpu_used_this_slice += 1;
    if (g.rr_cpu_used_this_slice >= rr_quantum_ticks_ && g.remaining_time > 0) {
      g.state = GuestState::READY;
      g.rr_cpu_used_this_slice = 0;
      ready_queue_.push_back(&g);
      log("[t=" + std::to_string(tick) + "] PREEMPT_RR pid=" + std::to_string(g.pid) +
          " quantum=" + std::to_string(rr_quantum_ticks_) + " -> READY");
      cpu_guest_ = nullptr;
      return;
    }
  }

  const bool work_done =
      (g.remaining_time == 0 && (g.visit_completed || g.requested_ride_id < 0));
  if (work_done) {
    g.state = GuestState::TERMINATED;
    wait_sum_ += g.total_wait_ticks;
    ++completed_;
    releaseMemoryFor(g, tick);
    log("[t=" + std::to_string(tick) + "] TERMINATED pid=" + std::to_string(g.pid) +
        " (cpu finished)");
    cpu_guest_ = nullptr;
  }
}

void Simulation::releaseMemoryFor(Guest& g, int tick) {
  if (!g.memory_allocated) {
    return;
  }
  memory_.release(g.memory_start, g.memory_requirement);
  g.memory_allocated = false;
  std::ostringstream os;
  os << "[t=" << tick << "] MEM_FREE pid=" << g.pid << " -"
     << g.memory_requirement << " used=" << memory_.used() << "/"
     << memory_.total() << " start=" << g.memory_start;
  g.memory_start = -1;
  log(os.str());
}

// Stops the main loop once every guest has reached TERMINATED.
bool Simulation::allGuestsFinished() const {
  for (const auto& gptr : guests_) {
    if (gptr->state != GuestState::TERMINATED) {
      return false;
    }
  }
  return true;
}

// Main driver: runs up to max_ticks minutes, or until all guests finish.
void Simulation::run(int max_ticks) {
  ticks_executed_ = 0;
  ride_busy_ticks_ = 0;
  log("=== Simulation start (1 tick = 1 minute, RR quantum = " +
      std::to_string(rr_quantum_ticks_) + " ticks) ===");
  std::vector<Guest*> disembarked_all;
  std::vector<Guest*> admitted_all;

  for (int t = 0; t < max_ticks; ++t) {
    ticks_executed_ = t + 1;
    admitNewArrivals(t);

    processRidePhase(t, disembarked_all, admitted_all);

    updateWaitMetrics();

    tryDispatchCpu(t);
    cpuTick(t);

    if (!cpu_guest_) {
      tryDispatchCpu(t);
    }

    if (allGuestsFinished()) {
      log("[t=" + std::to_string(t) + "] All guests finished; stopping simulation.");
      break;
    }
  }

  std::ostringstream summary;
  summary << "=== Simulation end: completed=" << completed_
          << " avg_wait_ticks=" << averageWait()
          << " avg_ride_utilization=" << averageRideUtilization() << " ===";
  log(summary.str());
}

// Three rides used by main: capacities and durations match the demo scenario design.
std::vector<std::unique_ptr<Ride>> makeDemoRides() {
  std::vector<std::unique_ptr<Ride>> v;
  v.push_back(std::make_unique<Ride>(0, "Carousel", 2, 3));
  v.push_back(std::make_unique<Ride>(1, "RollerCoaster", 1, 4));
  // Capacity 1 so a lone guest can finish (multi-seat rides need a full load before
  // the cycle starts; demo guests only include one Ferris rider).
  v.push_back(std::make_unique<Ride>(2, "Ferris", 1, 2));
  return v;
}

