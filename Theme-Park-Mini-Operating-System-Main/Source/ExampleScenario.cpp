#include "ExampleScenario.h"

std::vector<GuestProcess> ExampleScenario::loadScenario() {
    std::vector<GuestProcess> guests;

    guests.push_back({0, 0, 4, 4, 200, 2, 0, NEW});
    guests.push_back({1, 1, 3, 3, 128, 1, 1, NEW});
    guests.push_back({2, 2, 6, 6, 256, 3, 0, NEW});
    guests.push_back({3, 3, 2, 2, 128, 2, 1, NEW});
    guests.push_back({4, 4, 5, 5, 300, 0, 0, NEW});

    return guests;
}
