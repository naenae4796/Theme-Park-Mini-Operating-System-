cmake_minimum_required(VERSION 3.16)
project(theme_park_os CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(theme_park_os
  src/main.cpp
  src/Guest.cpp
  src/Ride.cpp
  src/Simulation.cpp
  src/MemoryManager.cpp
  src/ScenarioData.cpp
)

target_include_directories(theme_park_os PRIVATE include)
