# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name
project(Algorithms_FinalGroupProject)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add source files
add_executable(Algorithms_FinalGroupProject
    classes_AlgoFinalProj.cpp
    classes2_AFP.cpp
    scrapwork_AlgoFinalProj.cpp
    TestGraph.cpp
        test_curl_OSRM.cpp
        TestCases.cpp
        TestCases.cpp
        TestCases.cpp
)

# Link libraries if needed (example using vcpkg-installed libraries)
# find_package(cpr CONFIG REQUIRED)  # Example for linking cpr if installed via vcpkg
# target_link_libraries(Algorithms_FinalGroupProject PRIVATE cpr::cpr)
