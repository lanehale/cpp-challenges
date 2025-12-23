# Minimum required version of CMake
cmake_minimum_required(VERSION 3.20)

# Define the project name and language
project(CoordinateConversion CXX)

# Set the C++ standard to C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Explicitly find required packages
find_package(Boost REQUIRED)
find_package(libInterpolate CONFIG REQUIRED)

# Add executable target and source files
add_executable(CoordinateConversion LlaEcefConverter.cpp)

# Link the required libraries provided by vcpkg
target_link_libraries(CoordinateConversion PRIVATE 
    libInterpolate::Interpolate 
    Boost::boost
)
