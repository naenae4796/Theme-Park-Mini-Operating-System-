#pragma once
#include <vector>

// Fixed-size contiguous memory model for the MiniOS simulation.
// Guests must acquire memory before entering READY/RUNNING, and release it
// when they terminate.
class MemoryManager {
 public:
  explicit MemoryManager(int total_memory);

  bool allocate(int amount);
  void release(int start, int amount);

  int total() const { return total_memory_; }
  int used() const { return used_memory_; }
  int available() const { return total_memory_ - used_memory_; }
  int lastAllocationStart() const { return last_allocation_start_; }

 private:
  struct Block {
    int start = 0;
    int size = 0;
    bool free = true;
  };

  void mergeAdjacentFreeBlocks();

  int total_memory_ = 0;
  int used_memory_ = 0;
  int last_allocation_start_ = -1;
  std::vector<Block> blocks_;
};
