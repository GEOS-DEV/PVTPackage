# Serializers consider the classes of the PVT package and tranform them into json.
# They also obviously need json keys to properly map c++ and json fields.
set( pvt_tests_serializers_dependencies
     PVTPackage
     pvt_tests_constants
     nlohmann_json::nlohmann_json )

message( "Adding pvt_tests_serializers library" )
blt_add_library( NAME pvt_tests_serializers
                 SOURCES MultiphaseSystemProperties.cpp
                 HEADERS MultiphaseSystemProperties.hpp
                 DEPENDS_ON ${pvt_tests_serializers_dependencies} )
