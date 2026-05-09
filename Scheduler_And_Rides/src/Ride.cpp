#include "Ride.h"
#include "Guest.h"

// Ride implementation: boarding from CPU (tryBoard), cycle start, and disembark at cycle end.
// Per-tick behavior (timer, wait-queue admission) lives in Ride::tick in Ride.h (inline).

Ride::Ride(int id, std::string name, int capacity, int duration_ticks)
    : id_(id), name_(std::move(name)), capacity_(capacity), duration_ticks_(duration_ticks) {}

// Called while the guest holds the CPU and is RUNNING. If the ride is free and has room,
// the guest boards; otherwise they become WAITING and join the FIFO queue.
bool Ride::tryBoard(Guest& g) {
  if (g.state != GuestState::RUNNING) {
    return false;
  }
  if (g.riding) {
    return true;
  }

  if (busy_) {
    g.state = GuestState::WAITING;
    waiting_queue_.push_back(&g);
    return false;
  }

  if (static_cast<int>(current_riders_.size()) >= capacity_) {
    g.state = GuestState::WAITING;
    waiting_queue_.push_back(&g);
    return false;
  }

  current_riders_.push_back(&g);
  g.riding = true;
  g.state = GuestState::RIDING;
  start_cycle_if_loading_complete();
  return true;
}

// Marks the ride busy and starts the countdown for this cycle (see header for loading rules).
void Ride::start_cycle_if_loading_complete() {
  if (busy_ || current_riders_.empty()) {
    return;
  }

  // Start once there is at least one rider. Capacity still limits how many guests
  // can occupy the ride in a single cycle, while busy/free state still controls
  // blocking and FIFO waiting behavior. This avoids deadlock when the scenario
  // has fewer waiting guests than the ride capacity.
  busy_ = true;
  remaining_busy_time_ = duration_ticks_;
}

// End of cycle: clear riders, free the ride, return guests to the simulation for READY/TERM logic.
void Ride::finish_cycle(std::vector<Guest*>& out_disembarked) {
  for (Guest* g : current_riders_) {
    if (g) {
      g->riding = false;
      out_disembarked.push_back(g);
    }
  }
  current_riders_.clear();
  busy_ = false;
  remaining_busy_time_ = 0;
}

// Short status line for debugging (name plus BUSY countdown or FREE).
std::string Ride::statusString() const {
  return name_ + (busy_ ? "[BUSY " + std::to_string(remaining_busy_time_) + "]" : "[FREE]");
}
