#ifndef LOGGER_H
#define LOGGER_H

#include <string>

class Logger {
public:
    void log(const std::string &message);
    void logStateChange(int pid, const std::string &from, const std::string &to);
    void logMemoryAlloc(int pid, int amount, int used, int total);
    void logMemoryFree(int pid, int amount, int used, int total);
    void logRideEvent(int pid, int rideId, const std::string &event);
    void logSchedule(int pid, const std::string &policy);
};

#endif

