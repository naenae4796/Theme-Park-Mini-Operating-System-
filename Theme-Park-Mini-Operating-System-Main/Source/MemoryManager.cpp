#include "MemoryManager.h"

MemoryManager::MemoryManager(int total) {
    totalMemory = total;
    usedMemory = 0;
}

bool MemoryManager::allocate(int amount) {
    if (usedMemory + amount <= totalMemory) {
        usedMemory += amount;
        return true;
    }
    return false;
}

void MemoryManager::freeMemory(int amount) {
    usedMemory -= amount;
    if (usedMemory < 0) usedMemory = 0;
}

int MemoryManager::getUsedMemory() const {
    return usedMemory;
}

int MemoryManager::getAvailableMemory() const {
    return totalMemory - usedMemory;
}
