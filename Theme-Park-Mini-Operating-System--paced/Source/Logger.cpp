#include "Logger.h"
#include <iostream>

void Logger::log(const std::string &message) {
    std::cout << message << std::endl;
}

void Logger::logStateChange(int pid, const std::string &from, const std::string &to) {
    std::cout << "[STATE] pid=" << pid << " " << from << " -> " << to << std::endl;
}

void Logger::logMemoryAlloc(int pid, int amount, int used, int total) {
    std::cout << "[MEMORY_ALLOC] pid=" << pid 
              << " +" << amount 
              << " used=" << used << "/" << total << std::endl;
}

void Logger::logMemoryFree(int pid, int amount, int used, int total) {
    std::cout << "[MEMORY_FREE] pid=" << pid 
              << " -" << amount 
              << " used=" << used << "/" << total << std::endl;
}

void Logger::logRideEvent(int pid, int rideId, const std::string &event) {
    std::cout << "[RIDE] pid=" << pid 
              << " ride=" << rideId 
              << " event=" << event << std::endl;
}

void Logger::logSchedule(int pid, const std::string &policy) {
    std::cout << "[SCHEDULE] pid=" << pid 
              << " policy=" << policy << std::endl;
}
