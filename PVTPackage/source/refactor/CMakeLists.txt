include(FetchContent)

FetchContent_Declare(json
                     GIT_REPOSITORY https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent
                     GIT_TAG v3.7.3)

FetchContent_GetProperties(json)
if(NOT json_POPULATED)
  FetchContent_Populate(json)
  add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

set( PVTPackage_dump_headers
     JsonKeys.hpp
     passiveDataStructures/MultiphaseSystemProperties.hpp
     passiveDataStructures/PhaseModel.hpp
     passiveDataStructures/PhaseProperties.hpp
     passiveDataStructures/PropertyAndDerivatives.hpp
     passiveDataStructures/PVTEnums.hpp
     deserializers/ComponentProperties.hpp
     deserializers/Flash.hpp
     deserializers/MultiphaseSystemProperties.hpp
     deserializers/PhaseModels.hpp
     deserializers/PhaseProperties.hpp
     deserializers/PropertyAndDerivatives.hpp
     deserializers/PVTEnums.hpp
     serializers/BlackOilModels.hpp
     serializers/ComponentProperties.hpp
     serializers/CubicEoSPhaseModel.hpp
     serializers/DeadOilPhaseModel.hpp
     serializers/Flash.hpp
     serializers/PVTEnums.hpp
     serializers/PropertyAndDerivatives.hpp
)

set( PVTPackage_dump_sources
     JsonKeys.cpp
     deserializers/ComponentProperties.cpp
     deserializers/Flash.cpp
     deserializers/MultiphaseSystemProperties.cpp
     deserializers/PhaseModels.cpp
     deserializers/PhaseProperties.cpp
     deserializers/PropertyAndDerivatives.cpp
     serializers/BlackOilModels.cpp
     serializers/ComponentProperties.cpp
     serializers/CubicEoSPhaseModel.cpp
     serializers/DeadOilPhaseModel.cpp
     serializers/Flash.cpp
     serializers/PropertyAndDerivatives.cpp
)

set(PVTPackage_dump_target PVTPackageDump)
set(PVTPackage_dump_dependencies nlohmann_json::nlohmann_json PVTPackage)

message("adding PVTPackageDump library")
blt_add_library(NAME       ${PVTPackage_dump_target}
                SOURCES    ${PVTPackage_dump_sources}
                HEADERS    ${PVTPackage_dump_headers}
                DEPENDS_ON ${PVTPackage_dump_dependencies})

# Tests (poor cmake impl)
set( PVTPackage_dump_test_sources
     testFirst.cpp
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
