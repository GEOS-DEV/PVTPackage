set( pvt_tests_pds_headers
     BlackOilDeadOilApiInputs.hpp
     CompositionalApiInputs.hpp
     MultiphaseSystemProperties.hpp
     PropertyAndDerivatives.hpp
     PVTEnums.hpp
     )

set( pvt_tests_pds_sources
     BlackOilDeadOilApiInputs.cpp
     CompositionalApiInputs.cpp
     MultiphaseSystemProperties.cpp
     )

# Passive data structures are simple structures storing data.
# They should not depend on anything
message( "Adding pvt_tests_pds library" )
blt_add_library( NAME pvt_tests_pds
                 SOURCES ${pvt_tests_pds_sources}
                 HEADERS ${pvt_tests_pds_headers} )
