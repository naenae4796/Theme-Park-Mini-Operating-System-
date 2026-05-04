#pragma once
#include <string>

enum class GuestState { NEW, READY, RUNNING, WAITING, TERMINATED };

struct Guest {
  int pid = 0;
  int arrival_time = 0;
  int burst_time = 0;
  int remaining_time = 0;
  int memory_requirement = 0;
  int priority = 0;          // lower number = higher priority
  int requested_ride_id = -1;
  GuestState state = GuestState::NEW;

  // Runtime flags for Person 2's ride/scheduler behavior.
  bool visit_completed = false;
  bool riding = false;
  int rr_cpu_used_this_slice = 0;
  bool memory_allocated = false;
  bool memory_block_logged = false;

  // Metrics.
  int first_ready_tick = -1;
  int total_wait_ticks = 0;

  Guest() = default;
  Guest(int p, int arr, int burst, int mem, int pri, int rideId);

  std::string stateString() const;
};

// Adapter aliases to help map to the base project's terminology in docs/merge notes.
using ProcessState = GuestState;
