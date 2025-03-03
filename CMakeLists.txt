cmake_minimum_required(VERSION 3.10)
project(OctaWaveProcessor)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Add the library
add_library(audio_processor STATIC
        src/CoreAudioProcessor.cpp
)

# Include headers
target_include_directories(audio_processor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Add an executable
add_executable(OctaWaveProcessor src/main.cpp)

# Link the library to the executable
target_link_libraries(OctaWaveProcessor audio_processor)