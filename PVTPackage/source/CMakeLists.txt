set( PVTPackage_sources
     pvt/pvt.cpp
     MultiphaseSystem/BlackOilMultiphaseSystem.cpp
     MultiphaseSystem/ComponentProperties.cpp
     MultiphaseSystem/DeadOilMultiphaseSystem.cpp
     MultiphaseSystem/FreeWaterMultiphaseSystem.cpp
     MultiphaseSystem/MultiphaseSystem.cpp
     MultiphaseSystem/NegativeTwoPhaseMultiphaseSystem.cpp
     MultiphaseSystem/TrivialMultiphaseSystem.cpp
     MultiphaseSystem/MultiphaseSystemProperties/BlackOilDeadOilMultiphaseSystemProperties.cpp
     MultiphaseSystem/MultiphaseSystemProperties/BlackOilFlashMultiphaseSystemProperties.cpp
     MultiphaseSystem/MultiphaseSystemProperties/CompositionalMultiphaseSystemProperties.cpp
     MultiphaseSystem/MultiphaseSystemProperties/DeadOilFlashMultiphaseSystemProperties.cpp
     MultiphaseSystem/MultiphaseSystemProperties/FactorMultiphaseSystemProperties.cpp
     MultiphaseSystem/MultiphaseSystemProperties/FreeWaterFlashMultiphaseSystemProperties.cpp
     MultiphaseSystem/MultiphaseSystemProperties/NegativeTwoPhaseFlashMultiphaseSystemProperties.cpp
     MultiphaseSystem/MultiphaseSystemProperties/TrivialFlashMultiphaseSystemProperties.cpp
     MultiphaseSystem/PhaseSplitModel/BlackOilFlash.cpp
     MultiphaseSystem/PhaseSplitModel/CompositionalFlash.cpp
     MultiphaseSystem/PhaseSplitModel/DeadOilFlash.cpp
     MultiphaseSystem/PhaseSplitModel/FreeWaterFlash.cpp
     MultiphaseSystem/PhaseSplitModel/NegativeTwoPhaseFlash.cpp
     MultiphaseSystem/PhaseSplitModel/TrivialFlash.cpp
     MultiphaseSystem/PhaseModel/BlackOil/BlackOil_Utils.cpp
     MultiphaseSystem/PhaseModel/BlackOil/BlackOil_OilModel.cpp
     MultiphaseSystem/PhaseModel/BlackOil/DeadOil_PhaseModel.cpp
     MultiphaseSystem/PhaseModel/BlackOil/BlackOil_WaterModel.cpp
     MultiphaseSystem/PhaseModel/BlackOil/BlackOil_GasModel.cpp
     MultiphaseSystem/PhaseModel/CubicEOS/CubicEoSPhaseModel.cpp
     Utils/StringUtils.cpp
     Utils/LogMessage.cpp
     Utils/Logger.cpp
)

set( PVTPackage_headers
     pvt/pvt.hpp
     MultiphaseSystem/BlackOilMultiphaseSystem.hpp
     MultiphaseSystem/ComponentProperties.hpp
     MultiphaseSystem/DeadOilMultiphaseSystem.hpp
     MultiphaseSystem/FreeWaterMultiphaseSystem.hpp
     MultiphaseSystem/MultiphaseSystem.hpp
     MultiphaseSystem/NegativeTwoPhaseMultiphaseSystem.hpp
     MultiphaseSystem/TrivialMultiphaseSystem.hpp
     MultiphaseSystem/MultiphaseSystemProperties/BlackOilDeadOilMultiphaseSystemProperties.hpp
     MultiphaseSystem/MultiphaseSystemProperties/BlackOilFlashMultiphaseSystemProperties.hpp
     MultiphaseSystem/MultiphaseSystemProperties/CompositionalMultiphaseSystemProperties.hpp
     MultiphaseSystem/MultiphaseSystemProperties/DeadOilFlashMultiphaseSystemProperties.hpp
     MultiphaseSystem/MultiphaseSystemProperties/FactorMultiphaseSystemProperties.hpp
     MultiphaseSystem/MultiphaseSystemProperties/FreeWaterFlashMultiphaseSystemProperties.hpp
     MultiphaseSystem/MultiphaseSystemProperties/NegativeTwoPhaseFlashMultiphaseSystemProperties.hpp
     MultiphaseSystem/MultiphaseSystemProperties/TrivialFlashMultiphaseSystemProperties.hpp
     MultiphaseSystem/PhaseSplitModel/BlackOilFlash.hpp
     MultiphaseSystem/PhaseSplitModel/CompositionalFlash.hpp
     MultiphaseSystem/PhaseSplitModel/DeadOilFlash.hpp
     MultiphaseSystem/PhaseSplitModel/FreeWaterFlash.hpp
     MultiphaseSystem/PhaseSplitModel/NegativeTwoPhaseFlash.hpp
     MultiphaseSystem/PhaseSplitModel/TrivialFlash.hpp
     MultiphaseSystem/PhaseModel/BlackOil/BlackOilDeadOilProperties.hpp
     MultiphaseSystem/PhaseModel/BlackOil/BlackOil_GasModel.hpp
     MultiphaseSystem/PhaseModel/BlackOil/PVTGdata.hpp
     MultiphaseSystem/PhaseModel/BlackOil/PVTOdata.hpp
     MultiphaseSystem/PhaseModel/BlackOil/DeadOil_PhaseModel.hpp
     MultiphaseSystem/PhaseModel/BlackOil/BlackOil_WaterModel.hpp
     MultiphaseSystem/PhaseModel/BlackOil/BlackOil_Utils.hpp
     MultiphaseSystem/PhaseModel/BlackOil/PVDdata.hpp
     MultiphaseSystem/PhaseModel/BlackOil/PVTWdata.hpp
     MultiphaseSystem/PhaseModel/BlackOil/BlackOil_OilModel.hpp
     MultiphaseSystem/PhaseModel/CubicEOS/CubicEoSPhaseModel.hpp
     MultiphaseSystem/PVTEnums.hpp
     Utils/Logger.hpp
     Utils/LogMessage.hpp
     Utils/math.hpp
     Utils/FileUtils.hpp
     Utils/Assert.hpp
     Utils/StringUtils.hpp
)

# Expose includes
set( PVTPackage_INCLUDE_DIRS ${PROJECT_INC_DIR} CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE )

set( PVTPackage_target PVTPackage )

set( PVTPackage_dependencies "" )

if( ENABLE_PVTPACKAGE_TESTS )
  add_subdirectory( refactor )
  set( PVTPackage_dependencies ${PVTPackage_dependencies} PVTPackageDump )
endif()

message("adding PVTPackage library")
blt_add_library(NAME       ${PVTPackage_target}
                SOURCES    ${PVTPackage_sources}
                HEADERS    ${PVTPackage_headers}
                DEPENDS_ON ${PVTPackage_dependencies})

# FIXME only pvt PUBLIC, the rest should be private.
target_include_directories( ${PVTPackage_target} PUBLIC ${CMAKE_CURRENT_LIST_DIR} )
