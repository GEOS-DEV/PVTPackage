cmake_minimum_required (VERSION 3.1)

# # Options
# set(USE_MPI OFF CACHE BOOL "Enable MPI")

# C++11 enforced
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(PROJECT_NAME "Test1")
project(${PROJECT_NAME} C CXX)
set(SOURCE main.cpp)
add_executable(${PROJECT_NAME} ${SOURCE})
target_link_libraries(${PROJECT_NAME} PVTPackage)
include_directories(${PVTPackage_INCLUDE_DIRS})
