#include "ScenarioData.h"

// Builds the same guest list as the original course project (pids, arrivals, bursts,
// memory, priority, requested ride id) so demos stay comparable to the base zip.

std::vector<std::unique_ptr<Guest>> makeBaseScenarioGuests() {
  std::vector<std::unique_ptr<Guest>> guests;

  // Mirrors Theme-Park-Mini-Operating-System--main/Source/ExampleScenario.cpp
  guests.push_back(std::make_unique<Guest>(0, 0, 4, 200, 2, 0));
  guests.push_back(std::make_unique<Guest>(1, 1, 3, 128, 1, 1));
  guests.push_back(std::make_unique<Guest>(2, 2, 6, 256, 3, 0));
  guests.push_back(std::make_unique<Guest>(3, 3, 2, 128, 2, 1));
  guests.push_back(std::make_unique<Guest>(4, 4, 5, 300, 0, 0));

  return guests;
}
