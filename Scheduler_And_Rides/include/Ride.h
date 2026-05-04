#pragma once
#include "Guest.h"
#include <deque>
#include <string>
#include <vector>

// Ride = shared resource: capacity seats per cycle, fixed duration per cycle,
// busy/free, FIFO waiting queue for guests who arrive while busy/full.
//
// Loading rule: guests are added to current_riders while the ride is idle (not busy).
// A cycle starts once at least one guest has boarded (not only when full); the timer
// then runs for duration_ticks. Capacity still limits how many riders share a cycle.
class Ride {
 public:
  Ride(int id, std::string name, int capacity, int duration_ticks);

  int id() const { return id_; }
  const std::string& name() const { return name_; }
  int capacity() const { return capacity_; }
  int duration() const { return duration_ticks_; }

  bool busy() const { return busy_; }
  int remaining_busy_time() const { return remaining_busy_time_; }

  const std::vector<Guest*>& current_riders() const { return current_riders_; }
  const std::deque<Guest*>& waiting_queue() const { return waiting_queue_; }

  // Called from CPU step while guest is RUNNING (holds CPU). Boards into loading if
  // idle and not full; otherwise blocks (WAITING + FIFO queue).
  bool tryBoard(Guest& g);

  // Once per simulation tick: advance busy timer, finish cycles, admit WAITING guests.
  // out_disembarked: guests who completed a ride cycle (Simulation sets READY/TERM).
  // out_admitted_from_wait: guests moved from ride wait queue onto the ride this tick.
  // out_ride_was_operating: true if the ride was running a cycle at the start or end of this tick
  // (counts utilization for this simulation minute).
  inline void tick(std::vector<Guest*>& out_disembarked, std::vector<Guest*>& out_admitted_from_wait,
                   bool& out_ride_was_operating);

  std::string statusString() const;

 private:
  void start_cycle_if_loading_complete();
  void finish_cycle(std::vector<Guest*>& out_disembarked);

  int id_;
  std::string name_;
  int capacity_;
  int duration_ticks_;

  bool busy_ = false;
  int remaining_busy_time_ = 0;

  std::vector<Guest*> current_riders_;
  std::deque<Guest*> waiting_queue_;
};

inline void Ride::tick(std::vector<Guest*>& out_disembarked,
                       std::vector<Guest*>& out_admitted_from_wait, bool& out_ride_was_operating) {
  out_disembarked.clear();
  out_admitted_from_wait.clear();

  const bool busy_before = busy_;

  if (busy_) {
    if (remaining_busy_time_ > 0) {
      --remaining_busy_time_;
    }
    if (remaining_busy_time_ == 0) {
      finish_cycle(out_disembarked);
    }
  }

  while (!busy_ && !waiting_queue_.empty() &&
         static_cast<int>(current_riders_.size()) < capacity_) {
    Guest* g = waiting_queue_.front();
    waiting_queue_.pop_front();
    if (!g || g->state == GuestState::TERMINATED) {
      continue;
    }
    current_riders_.push_back(g);
    g->riding = true;
    g->state = GuestState::RIDING;
    out_admitted_from_wait.push_back(g);
  }
  start_cycle_if_loading_complete();

  out_ride_was_operating = busy_before || busy_;
}
