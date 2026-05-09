// osproject.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include "../Source/MemoryManager.h"
#include "../Source/Logger.h"
#include "../Source/ExampleScenario.h"
#include "../Source/GuestProcess.h"

int main() {
    Logger logger;
    MemoryManager memory(1024);

    std::cout << "=== Theme Park Mini OS Simulation Demo ===\n\n";

    auto guests = ExampleScenario::loadScenario();

    std::cout << "Loaded " << guests.size() << " guest processes.\n\n";

    for (auto &g : guests) {
        std::cout << "Guest PID=" << g.pid 
                  << " requires " << g.memoryRequirement 
                  << " units of memory.\n";
    }

    std::cout << "\n=== Allocating Memory ===\n";

    for (auto &g : guests) {
        if (memory.allocate(g.memoryRequirement)) {
            logger.logMemoryAlloc(g.pid, g.memoryRequirement, memory.getUsedMemory(), 1024);
            logger.logStateChange(g.pid, "NEW", "READY");
        } else {
            logger.log("pid=" + std::to_string(g.pid) + " BLOCKED: not enough memory");
        }
    }

    std::cout << "\n=== Freeing Memory ===\n";

    for (auto &g : guests) {
        memory.freeMemory(g.memoryRequirement);
        logger.logMemoryFree(g.pid, g.memoryRequirement, memory.getUsedMemory(), 1024);
    }

    std::cout << "\n=== Demo Complete ===\n";

    return 0;
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
