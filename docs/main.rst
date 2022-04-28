################################################################################
PVT Package Hierarchy
################################################################################

The architecture of the PVT package is as follows

.. uml::
   :caption: Architecture of the PVT package

   @startuml

   package PVT {

   package public_interface {

   abstract LimitedMultiphaseSystem #PaleTurquoise {
     + {abstract} getLimitedMultiphaseSystemProperties(): LimitedMultiphaseSystemProperties
   }

   abstract LimitedMultiphaseSystemProperties #Plum {
     + {abstract} getPhaseMoleFraction( phaseType ): ScalarPropertyAndDerivatives< double >
     + {abstract} getMoleComposition( phaseType ): ScalarPropertyAndDerivatives< double >
     + {abstract} getDensity( phaseType ): ScalarPropertyAndDerivatives< double >
     + {abstract} getMolecularWeight( phaseType ): ScalarPropertyAndDerivatives< double >
   }

   class MultiphaseSystemBuilder {
     + {static} Build( args... ): LimitedMultiphaseSystem
   }

   } /' end of package public_interface '/

   class MultiphaseSystem #PaleTurquoise {
     # Flash * flash
     + getLimitedMultiphaseSystemProperties(): LimitedMultiphaseSystemProperties
   }

   LimitedMultiphaseSystem <|-- MultiphaseSystem

   class FreeWaterFlashMultiphaseSystemProperties #Plum
   class NegativeTwoPhaseFlashMultiphaseSystemProperties #Plum
   class TrivialFlashMultiphaseSystemProperties #Plum
   class BlackOilFlashMultiphaseSystemProperties #Plum{
     + getOilPhaseModel(): BlackOil_OilModel
     + getGasPhaseModel(): BlackOil_GasModel
     + getLiquidWaterRichPhaseModel(): BlackOil_WaterModel
   }
   class DeadOilFlashMultiphaseSystemProperties #Plum {
     + getOilPhaseModel(): DeadOil_OilModel
     + getGasPhaseModel(): DeadOil_GasModel
     + getLiquidWaterRichPhaseModel(): BlackOil_WaterModel
   }

   LimitedMultiphaseSystemProperties <|-- BlackOilFlashMultiphaseSystemProperties
   LimitedMultiphaseSystemProperties <|-- DeadOilFlashMultiphaseSystemProperties
   LimitedMultiphaseSystemProperties <|-- NegativeTwoPhaseFlashMultiphaseSystemProperties
   NegativeTwoPhaseFlashMultiphaseSystemProperties<|-- FreeWaterFlashMultiphaseSystemProperties
   NegativeTwoPhaseFlashMultiphaseSystemProperties <|-- TrivialFlashMultiphaseSystemProperties

   Class FlashFactory {
     + {static} Build( args... ): Flash
   }

   abstract class Flash #PaleGreen {
     + {abstract} getLimitedMultiphaseSystemProperties(): LimitedMultiphaseSystemProperties
     + {abstract} ComputeEquilibrium(...)

   }
   abstract class CompositionalFlash  #PaleGreen
   class FreeWaterFlash #PaleGreen {
     # FreeWaterFlashMultiphaseSystemProperties * props
     + getLimitedMultiphaseSystemProperties(): FreeWaterFlashMultiphaseSystemProperties
     + ComputeEquilibrium(...)
   }
   class NegativeTwoPhaseFlash #PaleGreen {
     # NegativeTwoPhaseFlashMultiphaseSystemProperties * props
     + getLimitedMultiphaseSystemProperties(): NegativeTwoPhaseFlashMultiphaseSystemProperties
     + ComputeEquilibrium(...)
   }
   class TrivialFlash #PaleGreen {
     # TrivialFlashMultiphaseSystemProperties * props
     + getLimitedMultiphaseSystemProperties(): TrivialFlashMultiphaseSystemProperties
     + ComputeEquilibrium(...)
   }
   class BlackOilFlash #PaleGreen {
     # BlackOilFlashMultiphaseSystemProperties * props
     + getLimitedMultiphaseSystemProperties(): BlackOilFlashMultiphaseSystemProperties
     + ComputeEquilibrium(...)
   }
   class DeadOilFlash #PaleGreen {
     # DeadOilFlashMultiphaseSystemProperties * props
     + getLimitedMultiphaseSystemProperties(): DeadOilFlashMultiphaseSystemProperties
     + ComputeEquilibrium(...)
   }

   Flash <|-up- CompositionalFlash
   CompositionalFlash <|-up- FreeWaterFlash
   CompositionalFlash <|-up- NegativeTwoPhaseFlash
   CompositionalFlash <|-up- TrivialFlash
   Flash <|-up- BlackOilFlash
   Flash <|-up- DeadOilFlash

   FlashFactory <.. MultiphaseSystem
   Flash <.. MultiphaseSystem

   Flash <.. FlashFactory

   FreeWaterFlashMultiphaseSystemProperties <.. FreeWaterFlash
   NegativeTwoPhaseFlashMultiphaseSystemProperties  <.. NegativeTwoPhaseFlash
   TrivialFlashMultiphaseSystemProperties  <.. TrivialFlash
   BlackOilFlashMultiphaseSystemProperties  <.. BlackOilFlash
   DeadOilFlashMultiphaseSystemProperties  <.. DeadOilFlash

   class BlackOil_PhaseModel #LightSalmon
   class BlackOil_GasModel #LightSalmon
   class BlackOil_OilModel #LightSalmon
   class BlackOil_WaterModel #LightSalmon
   class DeadOil_PhaseModel #LightSalmon
   class DeadOil_GasModel #LightSalmon
   class DeadOil_OilModel #LightSalmon
   class CubicEoSPhaseModel #LightSalmon

   BlackOil_PhaseModel <|-up- BlackOil_GasModel
   BlackOil_PhaseModel <|-up- BlackOil_OilModel

   DeadOil_PhaseModel <|-up- DeadOil_GasModel
   DeadOil_PhaseModel <|-up- DeadOil_OilModel

   BlackOilFlashMultiphaseSystemProperties ..> BlackOil_GasModel
   BlackOilFlashMultiphaseSystemProperties  ..> BlackOil_OilModel
   BlackOilFlashMultiphaseSystemProperties ..> BlackOil_WaterModel
   DeadOilFlashMultiphaseSystemProperties  ..> DeadOil_GasModel
   DeadOilFlashMultiphaseSystemProperties  ..> DeadOil_OilModel
   DeadOilFlashMultiphaseSystemProperties  ..> BlackOil_WaterModel
   NegativeTwoPhaseFlashMultiphaseSystemProperties ..> CubicEoSPhaseModel
   TrivialFlashMultiphaseSystemProperties ..> CubicEoSPhaseModel
   FreeWaterFlashMultiphaseSystemProperties ..> CubicEoSPhaseModel

   note right of FlashFactory
     There are "links" between FlashFactory and its products.
     Registration of products in a factory is a classic pattern.
   end note

   } /' end of package PVT '/

   @enduml

The color scheme is:

* Green is for computational flash classes

* Purple is for data classes

* Orange is for fluid models (black oil, free water...)

* Light blue are for computational system (algorithms and data combined)

