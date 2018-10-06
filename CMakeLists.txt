#Definitions
cmake_minimum_required (VERSION 3.1)

# check if PVTPackage is build as a submodule or a separate project
get_directory_property(parent_dir PARENT_DIRECTORY)
if(parent_dir)
    set(is_submodule ON)
else()
    set(is_submodule OFF)
endif()

if(NOT is_submodule)

    project(PVTPackage LANGUAGES CXX C)

    # Check if BLT is available
    if (NOT EXISTS ${PROJECT_SOURCE_DIR}/cmake/blt/SetupBLT.cmake)
        message(FATAL_ERROR
            "The BLT submodule is not present. "
            "Run the following two commands in your git repository: \n"
            "    git submodule init \n "
            "    git submodule update" )
    endif()

    # Set CMake module path
    set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

    # Setup the build system
    include(blt/cmake/SetupBLT.cmake)
    include(cmake/PVTPackage.cmake)

    # Options
    option(COMPILE_EXAMPLES "Compile examples" ON)

endif()

# Add source
add_subdirectory(PVTPackage)

if(NOT is_submodule)

    # Add examples
    if(COMPILE_EXAMPLES)
        add_subdirectory(examples)
    endif(COMPILE_EXAMPLES)

endif()
