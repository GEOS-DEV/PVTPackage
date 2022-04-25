include( FetchContent )

FetchContent_Declare( json
                      GIT_REPOSITORY https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent
                      GIT_TAG v3.9.1 )

FetchContent_GetProperties( json )
if( NOT json_POPULATED )
  FetchContent_Populate( json )
  add_subdirectory( ${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL )
endif()

# I did not want to make one folder just for one file of constants
blt_add_library( NAME pvt_tests_constants
                 SOURCES JsonKeys.cpp
                 HEADERS JsonKeys.hpp
                 )

# Those directories contain some targets
add_subdirectory( passiveDataStructures )
add_subdirectory( serializers )
add_subdirectory( deserializers )

# We have these files containing elements for the tests
# But they do not contain test, so a dedicated target is done
set( pvt_tests_factor_dependencies
     gtest
     PVTPackage
     pvt_tests_pds
     )

message( "adding pvt_tests_factor library" )
blt_add_library( NAME pvt_tests_factor
                 SOURCES TestFactor.cpp
                 HEADERS TestFactor.hpp
                 DEPENDS_ON ${pvt_tests_factor_dependencies} )

# This part contain the real "test end points"
set( pvt_tests_sources
     testPublicApi.cpp
     )

set( pvt_tests_target pvt_tests )

set( pvt_tests_dependencies
     nlohmann_json::nlohmann_json
     gtest
     PVTPackage
     pvt_tests_factor
     pvt_tests_constants
     pvt_tests_pds
     pvt_tests_deserializers
     pvt_tests_serializers
     )

# Add gtest C++ based tests
foreach( test ${pvt_tests_sources} )
  get_filename_component( test_name ${test} NAME_WE )
  blt_add_executable( NAME ${test_name}
                      SOURCES ${test}
                      OUTPUT_DIR ${TEST_OUTPUT_DIRECTORY}
                      DEPENDS_ON ${pvt_tests_dependencies} )
  blt_add_test( NAME ${test_name}
                COMMAND ${test_name} )
endforeach()
