#Definitions
cmake_minimum_required (VERSION 3.1)

project(PVTPackage LANGUAGES CXX C)

include(cmake/blt/SetupBLT.cmake)

include(cmake/PVTPackage.cmake)

#Path
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

#Options
option(COMPILE_EXAMPLES "Compile examples" ON)

#PVTPACKAGE
add_subdirectory(PVTPackage)

# #Examples
if(COMPILE_EXAMPLES)
    add_subdirectory(examples)
endif(COMPILE_EXAMPLES)




