cmake_minimum_required(VERSION 3.15)
project(TetrisGame VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files
set(SOURCES
    src/main.cpp
    src/Tetromino.cpp
)

# Create executable
add_executable(TetrisGame ${SOURCES})

# Include directories
target_include_directories(TetrisGame PRIVATE src)

# Windows-specific settings
if(WIN32)
    # Enable console window for debugging
    set_target_properties(TetrisGame PROPERTIES
        WIN32_EXECUTABLE FALSE
    )
endif()
