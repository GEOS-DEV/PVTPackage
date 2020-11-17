/*
 * ------------------------------------------------------------------------------------------------------------
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 * Copyright (c) 2018-2020 Lawrence Livermore National Security LLC
 * Copyright (c) 2018-2020 The Board of Trustees of the Leland Stanford Junior University
 * Copyright (c) 2018-2020 Total, S.A
 * Copyright (c) 2020-     GEOSX Contributors
 * All right reserved
 *
 * See top level LICENSE, COPYRIGHT, CONTRIBUTORS, NOTICE, and ACKNOWLEDGEMENTS files for details.
 * ------------------------------------------------------------------------------------------------------------
 */

#ifndef PVTPACKAGE_COMPOSITIONALMULTIPHASESYSTEMPROPERTIES_HPP
#define PVTPACKAGE_COMPOSITIONALMULTIPHASESYSTEMPROPERTIES_HPP

#include "MultiphaseSystem/MultiphaseSystemProperties/FactorMultiphaseSystemProperties.hpp"
#include "MultiphaseSystem/ComponentProperties.hpp"

#include "MultiphaseSystem/PhaseModel/CubicEOS/CubicEoSPhaseModel.hpp"

#include "pvt/pvt.hpp"
#include <map>

#include <vector>

namespace PVTPackage
{

class CompositionalMultiphaseSystemProperties : public FactorMultiphaseSystemProperties
{
public:

  CompositionalMultiphaseSystemProperties( const std::vector< pvt::PHASE_TYPE > & phases,
                                           const std::vector< pvt::EOS_TYPE > & eosTypes,
                                           ComponentProperties const & componentProperties );

  const ComponentProperties & getComponentProperties() const;

  // FIXME TODO This should not be there (data), but part of Flash (algorithm)
  const CubicEoSPhaseModel & getCubicEoSPhaseModel( const pvt::PHASE_TYPE & phase ) const;

  double const & getTemperature() const;

  void setTemperature( double const & temperature );

  void setOilMoleComposition( std::vector< double > const & moleComposition );

  void setGasMoleComposition( std::vector< double > const & moleComposition );

  void setOilFraction( double const & fraction );

  void setGasFraction( double const & fraction );

  /**
   * @brief Sets the properties computed by the model for given @p phase.
   * @param phase The phase for which we want to set properties.
   * @param properties The new values.
   *
   * Mainly defines mass density, mole density, molecular weight,
   * compressibility factor and ln fugacity coefficients.
   */
  void setModelProperties( const pvt::PHASE_TYPE & phase,
                           const CubicEoSPhaseModel::Properties & properties );

  void setPhaseMoleFractionDT( pvt::PHASE_TYPE const & phase,
                               double const & value );

  void setMolecularWeightDT( pvt::PHASE_TYPE const & phase,
                             double const & value );

  void setMoleDensityDT( pvt::PHASE_TYPE const & phase,
                         double const & value );

  void setMassDensityDT( pvt::PHASE_TYPE const & phase,
                         double const & value );

  void setMoleCompositionDT( pvt::PHASE_TYPE const & phase,
                             std::vector< double > const & value );

protected:

  std::map< pvt::PHASE_TYPE, double > m_compressibilityFactor;
  std::map< pvt::PHASE_TYPE, std::vector< double > > m_lnFugacity;

  double m_temperature;
  ComponentProperties m_componentProperties;
  std::map< pvt::PHASE_TYPE, CubicEoSPhaseModel > m_phaseModels;
};

}

#endif //PVTPACKAGE_COMPOSITIONALMULTIPHASESYSTEMPROPERTIES_HPP
