#pragma once
#include "Guest.h"
#include <algorithm>
#include <deque>

// Which rule to use when removing one guest from the READY queue for CPU dispatch.
enum class SchedulingPolicy { FCFS, Priority, RoundRobin };

// Picks the next guest to run from an explicit std::deque<Guest*> (the READY queue).
//
// FCFS: pop the front — whoever entered READY first (deque order) runs next.
// Round Robin: same pick as FCFS; the 2-tick quantum and preemption happen in
//   Simulation::cpuTick, which puts the guest back on the back of this deque.
// Priority: scan the whole deque and remove the "best" guest by priority, then
//   arrival time, then pid (see pickNext body).
class Scheduler {
 public:
  explicit Scheduler(SchedulingPolicy sched_policy) : policy_(sched_policy) {}

  void setPolicy(SchedulingPolicy p) { policy_ = p; }
  SchedulingPolicy policy() const { return policy_; }

  // Remove and return one guest from `ready`, or nullptr if empty.
  // Caller (Simulation) then sets that guest RUNNING and assigns the CPU.
  inline Guest* pickNext(std::deque<Guest*>& ready) {
    if (ready.empty()) {
      return nullptr;
    }

    // FCFS and RR: strict FIFO — front of the deque is the next process.
    if (policy_ == SchedulingPolicy::FCFS || policy_ == SchedulingPolicy::RoundRobin) {
      Guest* g = ready.front();
      ready.pop_front();
      return g;
    }

    // Priority: choose the guest with lowest priority number (most urgent).
    // Ties: smaller arrival_time wins; if still tied, smaller pid wins.
    // min_element returns the first minimal element under this ordering.
    auto it = std::min_element(ready.begin(), ready.end(), [](Guest* a, Guest* b) {
      if (a->priority != b->priority) {
        return a->priority < b->priority;
      }
      if (a->arrival_time != b->arrival_time) {
        return a->arrival_time < b->arrival_time;
      }
      return a->pid < b->pid;
    });
    Guest* g = *it;
    ready.erase(it);
    return g;
  }

 private:
  SchedulingPolicy policy_;
};
