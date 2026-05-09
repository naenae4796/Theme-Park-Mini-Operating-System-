#include <iostream>
#include "../MemoryManager.h"
#include "../Logger.h"
#include "../ExampleScenario.h"

int main() {
    Logger logger;
    MemoryManager memory(1024);

    // Load example scenario
    auto guests = ExampleScenario::loadScenario();

    std::cout << "=== Testing Example Scenario ===" << std::endl;

    for (auto &g : guests) {
        std::cout << "Guest PID=" << g.pid 
                  << " requires " << g.memoryRequirement 
                  << " units of memory." << std::endl;
    }

    std::cout << "\n=== Testing Memory Allocation ===" << std::endl;

    for (auto &g : guests) {
        bool allocated = memory.allocate(g.memoryRequirement);

        if (allocated) {
            logger.logMemoryAlloc(
                g.pid,
                g.memoryRequirement,
                memory.getUsedMemory(),
                1024
            );
            logger.logStateChange(g.pid, "NEW", "READY");
        } else {
            logger.log("pid=" + std::to_string(g.pid) + " BLOCKED: not enough memory");
        }
    }

    std::cout << "\n=== Testing Memory Freeing ===" << std::endl;

    for (auto &g : guests) {
        memory.freeMemory(g.memoryRequirement);
        logger.logMemoryFree(
            g.pid,
            g.memoryRequirement,
            memory.getUsedMemory(),
            1024
        );
    }

    std::cout << "\n=== Test Complete ===" << std::endl;

    return 0;
}
