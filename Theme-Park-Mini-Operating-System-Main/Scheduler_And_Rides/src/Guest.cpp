#include "Guest.h"

// Guest models one process: CPU burst, memory need, priority, and an optional ride request.
// State (NEW/READY/RUNNING/WAITING/TERMINATED) is updated by the simulation and rides.

Guest::Guest(int p, int arr, int burst, int mem, int pri, int rideId)
    : pid(p),
      arrival_time(arr),
      burst_time(burst),
      remaining_time(burst),
      memory_requirement(mem),
      priority(pri),
      requested_ride_id(rideId) {}

// Human-readable name for logging and demo output.
std::string Guest::stateString() const {
  switch (state) {
    case GuestState::NEW:
      return "NEW";
    case GuestState::READY:
      return "READY";
    case GuestState::RUNNING:
      return "RUNNING";
    case GuestState::WAITING:
      return "WAITING";
    case GuestState::TERMINATED:
      return "TERMINATED";
  }
  return "?";
}
