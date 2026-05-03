#pragma once
#include "Guest.h"
#include <memory>
#include <vector>

// Loads the same guest values used by the untouched base zip's ExampleScenario.
std::vector<std::unique_ptr<Guest>> makeBaseScenarioGuests();
