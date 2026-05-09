#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

class MemoryManager {
private:
    int totalMemory;
    int usedMemory;

public:
    MemoryManager(int total = 1024);

    bool allocate(int amount);
    void freeMemory(int amount);
    int getUsedMemory() const;
    int getAvailableMemory() const;
};

#endif
