cmake_minimum_required(VERSION 3.24)
include(FindPkgConfig)
pkg_check_modules(SDL2TTF REQUIRED SDL2_ttf)
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
add_executable(pac ${SOURCE_FILES})

# Link SDL2 to the project
target_link_libraries(pac ${SDL2_LIBRARIES})

# Include SDL2_ttf headers
include_directories(${SDL2TTF_INCLUDE_DIRS})

# Link SDL2_ttf to the project
target_link_libraries(pac ${SDL2TTF_LIBRARIES})

