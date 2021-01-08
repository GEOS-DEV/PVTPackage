include(FetchContent)

FetchContent_Declare(json
                     GIT_REPOSITORY https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent
                     GIT_TAG v3.9.1)

FetchContent_GetProperties(json)
if(NOT json_POPULATED)
  FetchContent_Populate(json)
  add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()


find_package(OpenMP)

set( PVTPackage_dump_headers
     JsonKeys.hpp
     TestFactor.hpp
     deserializers/BlackOilDeadOilApiInputs.hpp
     deserializers/CompositionalApiInputs.hpp
     deserializers/MultiphaseSystemProperties.hpp
     deserializers/PropertyAndDerivatives.hpp
     deserializers/PVTEnums.hpp
     serializers/MultiphaseSystemProperties.hpp
     passiveDataStructures/BlackOilDeadOilApiInputs.hpp
     passiveDataStructures/CompositionalApiInputs.hpp
     passiveDataStructures/MultiphaseSystemProperties.hpp
     passiveDataStructures/PropertyAndDerivatives.hpp
     passiveDataStructures/PVTEnums.hpp
     )

set( PVTPackage_dump_sources
     JsonKeys.cpp
     TestFactor.cpp
     deserializers/BlackOilDeadOilApiInputs.cpp
     deserializers/CompositionalApiInputs.cpp
     deserializers/PropertyAndDerivatives.cpp
     serializers/MultiphaseSystemProperties.cpp
     passiveDataStructures/BlackOilDeadOilApiInputs.cpp
     passiveDataStructures/CompositionalApiInputs.cpp
     passiveDataStructures/MultiphaseSystemProperties.cpp
     )

set(PVTPackage_dump_target PVTPackageDump)
set(PVTPackage_dump_dependencies nlohmann_json::nlohmann_json gtest PVTPackage)

message("adding PVTPackageDump library")
blt_add_library(NAME       ${PVTPackage_dump_target}
                SOURCES    ${PVTPackage_dump_sources}
                HEADERS    ${PVTPackage_dump_headers}
                DEPENDS_ON ${PVTPackage_dump_dependencies})

# Tests (poor cmake impl)
set( PVTPackage_dump_test_sources
     testPublicApi.cpp
     testParallel.cpp
)

#
# Add gtest C++ based tests
#
foreach(test ${PVTPackage_dump_test_sources})
  get_filename_component( test_name ${test} NAME_WE )
  blt_add_executable( NAME ${test_name}
                      SOURCES ${test}
                      OUTPUT_DIR ${TEST_OUTPUT_DIRECTORY}
                      DEPENDS_ON gtest PVTPackageDump )

  blt_add_test( NAME ${test_name}
                COMMAND ${test_name} )

endforeach()
