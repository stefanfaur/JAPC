cmake_minimum_required(VERSION 3.24)
project(pac C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Locate SDL2 library
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Define source files
set(SOURCE_FILES main.c)

# Create an executable
add_executable(Pacman ${SOURCE_FILES})

# Link SDL2 to the project
target_link_libraries(Pacman ${SDL2_LIBRARIES})

