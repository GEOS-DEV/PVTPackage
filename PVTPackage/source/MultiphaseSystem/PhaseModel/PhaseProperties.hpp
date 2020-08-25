/*
 * ------------------------------------------------------------------------------------------------------------
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 * Copyright (c) 2018-2020 Lawrence Livermore National Security LLC
 * Copyright (c) 2018-2020 The Board of Trustees of the Leland Stanford Junior University
 * Copyright (c) 2018-2020 Total, S.A
 * Copyright (c) 2020-     GEOSX Contributors
 * All rights reserved
 *
 * See top level LICENSE, COPYRIGHT, CONTRIBUTORS, NOTICE, and ACKNOWLEDGEMENTS files for details.
 * ------------------------------------------------------------------------------------------------------------
 */

#pragma once

#include "MultiphaseSystem/PropertyAndDerivatives.hpp"

namespace PVTPackage
{

struct PhaseProperties
{
  PhaseProperties(std::size_t ncomponents)
  : MolecularWeight(ncomponents),
    CompressibilityFactor(ncomponents),
    MoleComposition(ncomponents,ncomponents),
    LnFugacityCoefficients(ncomponents, ncomponents),
    MoleDensity(ncomponents),
    MassDensity(ncomponents),
    Viscosity(0),
    MassEnthalpy(0),
    Compressibility(0)
  {
  }

  PhaseProperties()
  : MolecularWeight(0),
    CompressibilityFactor(0),
    MoleComposition(0, 0),
    LnFugacityCoefficients(0, 0),
    MoleDensity(0),
    MassDensity(0),
    Viscosity(0),
    MassEnthalpy(0),
    Compressibility(0)
  {

//    auto df = 2; //TODO: don't know why I need this default constructor. Nothing goes through it but compiler crashes
//    df++;
  }

  void UpdateDerivative_dP_FiniteDifference(const PhaseProperties& props_eps, double epsilon)
  {
    MolecularWeight.dP = (props_eps.MolecularWeight.value - MolecularWeight.value) / epsilon;
    CompressibilityFactor.dP = (props_eps.CompressibilityFactor.value - CompressibilityFactor.value) / epsilon;
    MoleDensity.dP = (props_eps.MoleDensity.value - MoleDensity.value) / epsilon;
    MassDensity.dP = (props_eps.MassDensity.value - MassDensity.value) / epsilon;
    for (std::size_t i = 0; i < MoleComposition.value.size(); ++i)
    {
      MoleComposition.dP[i] = (props_eps.MoleComposition.value[i] - MoleComposition.value[i]) / epsilon;
      LnFugacityCoefficients.dP[i] = (props_eps.LnFugacityCoefficients.value[i] - LnFugacityCoefficients.value[i]) / epsilon;
    }
  }

  void UpdateDerivative_dT_FiniteDifference(const PhaseProperties& props_eps, double epsilon)
  {
    MolecularWeight.dT = (props_eps.MolecularWeight.value - MolecularWeight.value) / epsilon;
    CompressibilityFactor.dT = (props_eps.CompressibilityFactor.value - CompressibilityFactor.value) / epsilon;
    MoleDensity.dT = (props_eps.MoleDensity.value - MoleDensity.value) / epsilon;
    MassDensity.dT = (props_eps.MassDensity.value - MassDensity.value) / epsilon;
    for (std::size_t i = 0; i < MoleComposition.value.size(); ++i)
    {
      MoleComposition.dT[i] = (props_eps.MoleComposition.value[i] - MoleComposition.value[i]) / epsilon;
      LnFugacityCoefficients.dT[i] = (props_eps.LnFugacityCoefficients.value[i] - LnFugacityCoefficients.value[i]) / epsilon;
    }
  }

  void UpdateDerivative_dz_FiniteDifference(std::size_t icomponent, const PhaseProperties& props_eps, double epsilon)
  {
    MolecularWeight.dz[icomponent] = (props_eps.MolecularWeight.value - MolecularWeight.value) / epsilon;
    CompressibilityFactor.dz[icomponent] = (props_eps.CompressibilityFactor.value - CompressibilityFactor.value) / epsilon;
    MoleDensity.dz[icomponent] = (props_eps.MoleDensity.value - MoleDensity.value) / epsilon;
    MassDensity.dz[icomponent] = (props_eps.MassDensity.value - MassDensity.value) / epsilon;
    for (std::size_t i = 0; i < MoleComposition.value.size(); ++i)
    {
      MoleComposition.dz[i][icomponent] = (props_eps.MoleComposition.value[i] - MoleComposition.value[i]) / epsilon;
      LnFugacityCoefficients.dz[i][icomponent] = (props_eps.LnFugacityCoefficients.value[i] - LnFugacityCoefficients.value[i]) / epsilon;
    }
  }


  //Properties
  //--Constant
  ScalarPropertyAndDerivatives<double> MolecularWeight;
  //--From Flash
  ScalarPropertyAndDerivatives<double> CompressibilityFactor;
  VectorPropertyAndDerivatives<double> MoleComposition;
  VectorPropertyAndDerivatives<double> LnFugacityCoefficients;
  //--Indirectly from flash
  ScalarPropertyAndDerivatives<double> MoleDensity;
  ScalarPropertyAndDerivatives<double> MassDensity;

  double Viscosity;
  double MassEnthalpy;
  double Compressibility;

};

}
