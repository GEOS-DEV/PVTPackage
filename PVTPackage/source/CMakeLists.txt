set(PVTPackage_target PVTPackage)

set(PVTPackage_headers
    PVTPackage.hpp
    MultiphaseSystem/BlackOilMultiphaseSystem.hpp
    MultiphaseSystem/BlackOilProperties.hpp
    MultiphaseSystem/ComponentProperties.hpp
    MultiphaseSystem/CompositionalMultiphaseSystem.hpp
    MultiphaseSystem/FlashVariables.hpp
    MultiphaseSystem/MultiphaseSystem.hpp
    MultiphaseSystem/MultiphaseSystemProperties.hpp
    MultiphaseSystem/PhaseModelOutputVariables.hpp
    MultiphaseSystem/PVTEnums.hpp
    MultiphaseSystem/PhaseModel/PhaseModel.hpp
    MultiphaseSystem/PhaseModel/PhaseProperties.hpp
    MultiphaseSystem/PhaseModel/BlackOil/BlackOil_GasModel.hpp
    MultiphaseSystem/PhaseModel/BlackOil/BlackOil_OilModel.hpp
    MultiphaseSystem/PhaseModel/BlackOil/BlackOil_WaterModel.hpp
    MultiphaseSystem/PhaseModel/BlackOil/BlackOil_Utils.hpp
    MultiphaseSystem/PhaseModel/BlackOil/PVTOdata.hpp
    MultiphaseSystem/PhaseModel/CubicEOS/CubicEoSPhaseModel.hpp
    MultiphaseSystem/PhaseSplitModel/BlackOilFlash.hpp
    MultiphaseSystem/PhaseSplitModel/CompositionalFlash.hpp
    MultiphaseSystem/PhaseSplitModel/FreeWaterFlash.hpp
    MultiphaseSystem/PhaseSplitModel/TrivialFlash.hpp
    Utils/Assert.hpp
    Utils/File.hpp
    Utils/Logger.hpp
    Utils/LogMessage.hpp
    Utils/macros.hpp
    Utils/math.hpp
    Utils/StringUtils.hpp
    Utils/Timer.hpp
    Utils/Types.hpp
    Utils/utils.hpp)

set(PVTPackage_sources
    PVTPackage.cpp
    MultiphaseSystem/BlackOilMultiphaseSystem.cpp
    MultiphaseSystem/CompositionalMultiphaseSystem.cpp
    MultiphaseSystem/MultiphaseSystem.cpp
    MultiphaseSystem/PhaseModel/PhaseModel.cpp
    MultiphaseSystem/PhaseModel/BlackOil/BlackOil_GasModel.cpp
    MultiphaseSystem/PhaseModel/BlackOil/BlackOil_OilModel.cpp
    MultiphaseSystem/PhaseModel/BlackOil/BlackOil_WaterModel.cpp
    MultiphaseSystem/PhaseModel/BlackOil/BlackOil_Utils.cpp
    MultiphaseSystem/PhaseModel/CubicEOS/CubicEoSPhaseModel.cpp
    MultiphaseSystem/PhaseSplitModel/BlackOilFlash.cpp
    MultiphaseSystem/PhaseSplitModel/CompositionalFlash.cpp
    MultiphaseSystem/PhaseSplitModel/TrivialFlash.cpp
    Utils/File.cpp
    Utils/Logger.cpp
    Utils/LogMessage.cpp
    Utils/StringUtils.cpp
    Utils/Timer.cpp
    )

set(PVTPackage_dependencies "")

message("adding PVTPackage library")
blt_add_library(NAME                  ${PVTPackage_target}
                SOURCES               ${PVTPackage_sources}
                HEADERS               ${PVTPackage_headers}
                HEADERS_OUTPUT_SUBDIR PVTPackage
                DEPENDS_ON            ${PVTPackage_dependencies})

target_include_directories(${PVTPackage_target} PUBLIC ${CMAKE_CURRENT_LIST_DIR})