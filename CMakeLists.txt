cmake_minimum_required(VERSION 3.10)
project(OctaWaveProcessor)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Create a STATIC library instead of an executable
add_library(OctaWaveProcessor STATIC
        src/CoreAudioProcessor.cpp
)

# Ensure headers are available for inclusion
target_include_directories(OctaWaveProcessor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)