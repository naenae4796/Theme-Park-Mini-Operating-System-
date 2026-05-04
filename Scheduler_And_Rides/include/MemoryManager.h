#pragma once

// Fixed-size contiguous memory model for the MiniOS simulation.
// Guests must acquire memory before entering READY/RUNNING, and release it
// when they terminate.
class MemoryManager {
 public:
  explicit MemoryManager(int total_memory);

  bool allocate(int amount);
  void release(int amount);

  int total() const { return total_memory_; }
  int used() const { return used_memory_; }
  int available() const { return total_memory_ - used_memory_; }

 private:
  int total_memory_ = 0;
  int used_memory_ = 0;
};
