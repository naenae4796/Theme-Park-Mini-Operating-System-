#include "MemoryManager.h"

MemoryManager::MemoryManager(int total_memory) : total_memory_(total_memory) {}

bool MemoryManager::allocate(int amount) {
  if (amount <= 0) {
    return true;
  }
  if (used_memory_ + amount > total_memory_) {
    return false;
  }
  used_memory_ += amount;
  return true;
}

void MemoryManager::release(int amount) {
  if (amount <= 0) {
    return;
  }
  used_memory_ -= amount;
  if (used_memory_ < 0) {
    used_memory_ = 0;
  }
}
