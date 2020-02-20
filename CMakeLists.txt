#Definitions
cmake_minimum_required (VERSION 3.1)

project(PVTPackage LANGUAGES CXX C)

################################
# BLT
################################
if (NOT BLT_LOADED)
  if (DEFINED BLT_SOURCE_DIR)
      # Support having a shared BLT outside of the repository if given a BLT_SOURCE_DIR

      if (NOT EXISTS ${BLT_SOURCE_DIR}/SetupBLT.cmake)
          message(FATAL_ERROR "Given BLT_SOURCE_DIR does not contain SetupBLT.cmake")
      endif()
  else()
      # Use internal 'blt' submodule path if BLT_SOURCE_DIR not provided
      set(BLT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/cmake/blt" CACHE PATH "")
      if (NOT EXISTS ${BLT_SOURCE_DIR}/SetupBLT.cmake)
          message(FATAL_ERROR
              "Cannot locate BLT. "
              "Either run the following two commands in your git repository: \n"
              "    git submodule init\n"
              "    git submodule update\n"
              "Or add -DBLT_SOURCE_DIR=/path/to/blt to your CMake command." )
      endif()
  endif()

  include(${BLT_SOURCE_DIR}/SetupBLT.cmake)
endif()

# check if PVTPackage is build as a submodule or a separate project
get_directory_property(parent_dir PARENT_DIRECTORY)
if(parent_dir)
    set(is_submodule ON)
else()
    set(is_submodule OFF)
endif()

if(NOT is_submodule)

    # Set CMake module path
    set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake ${CMAKE_MODULE_PATH})

    # Setup the build system
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
