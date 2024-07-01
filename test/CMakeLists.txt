cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(EVSim VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the executable
add_executable(ev_sim main.cpp)

# Link the pigpio library to the executable
target_link_libraries(ev_sim pigpio rt)
