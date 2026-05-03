#ifndef EXAMPLE_SCENARIO_H
#define EXAMPLE_SCENARIO_H

#include <vector>
#include "GuestProcess.h"  // Person A provides this

class ExampleScenario {
public:
    static std::vector<GuestProcess> loadScenario();
};

#endif
