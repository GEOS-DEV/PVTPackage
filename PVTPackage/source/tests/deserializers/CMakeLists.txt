set( pvt_tests_deserializers_headers
     BlackOilDeadOilApiInputs.hpp
     CompositionalApiInputs.hpp
     MultiphaseSystemProperties.hpp
     PropertyAndDerivatives.hpp
     PVTEnums.hpp
     )

set( pvt_tests_deserializers_sources
     BlackOilDeadOilApiInputs.cpp
     CompositionalApiInputs.cpp
     PropertyAndDerivatives.cpp
     )

# The deserializers need to transform json into passive data structures.
# They also need some keys/constants to redirect the correct properties to the correct object type.
set( pvt_tests_deserializers_dependencies
     pvt_tests_constants
     pvt_tests_pds
     nlohmann_json::nlohmann_json )

message( "Adding pvt_tests_deserializers library" )
blt_add_library( NAME pvt_tests_deserializers
                 SOURCES ${pvt_tests_deserializers_sources}
                 HEADERS ${pvt_tests_deserializers_headers}
                 DEPENDS_ON ${pvt_tests_deserializers_dependencies} )
