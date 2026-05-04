#include "MemoryManager.h"

MemoryManager::MemoryManager(int total_memory) : total_memory_(total_memory) {
  blocks_.push_back(Block{0, total_memory, true});
}

bool MemoryManager::allocate(int amount) {
  last_allocation_start_ = -1;
  if (amount <= 0) {
    last_allocation_start_ = 0;
    return true;
  }

  // First-fit: use the first free contiguous block large enough for this guest.
  for (std::size_t i = 0; i < blocks_.size(); ++i) {
    auto& block = blocks_[i];
    if (!block.free || block.size < amount) {
      continue;
    }

    last_allocation_start_ = block.start;
    if (block.size == amount) {
      block.free = false;
    } else {
      const Block remainder{block.start + amount, block.size - amount, true};
      block.size = amount;
      block.free = false;
      blocks_.insert(blocks_.begin() + static_cast<long long>(i + 1), remainder);
    }
    used_memory_ += amount;
    return true;
  }

  return false;
}

void MemoryManager::release(int start, int amount) {
  if (amount <= 0) {
    return;
  }

  for (auto& block : blocks_) {
    if (block.start == start && !block.free) {
      block.free = true;
      used_memory_ -= amount;
      if (used_memory_ < 0) {
        used_memory_ = 0;
      }
      mergeAdjacentFreeBlocks();
      return;
    }
  }
}

void MemoryManager::mergeAdjacentFreeBlocks() {
  for (std::size_t i = 0; i + 1 < blocks_.size();) {
    if (blocks_[i].free && blocks_[i + 1].free) {
      blocks_[i].size += blocks_[i + 1].size;
      blocks_.erase(blocks_.begin() + static_cast<long long>(i + 1));
    } else {
      ++i;
    }
  }
}
