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

#ifndef PVTPACKAGE_FACTORMULTIPHASESYSTEMPROPERTIES_HPP
#define PVTPACKAGE_FACTORMULTIPHASESYSTEMPROPERTIES_HPP

#include "pvt/pvt.hpp"

#include <map>

namespace PVTPackage
{

class FactorMultiphaseSystemProperties : public pvt::MultiphaseSystemProperties
{
public:

  FactorMultiphaseSystemProperties( const std::vector< pvt::PHASE_TYPE > & phases,
                                    std::size_t nComponents );

  pvt::ScalarPropertyAndDerivatives< double > const & getMassDensity( pvt::PHASE_TYPE const & phase ) const final;

  pvt::ScalarPropertyAndDerivatives< double > const & getViscosity( pvt::PHASE_TYPE const & phase ) const final;
  
  pvt::VectorPropertyAndDerivatives< double > const & getMoleComposition( pvt::PHASE_TYPE const & phase ) const final;

  pvt::ScalarPropertyAndDerivatives< double > const & getMoleDensity( pvt::PHASE_TYPE const & phase ) const final;

  pvt::ScalarPropertyAndDerivatives< double > const & getMolecularWeight( pvt::PHASE_TYPE const & phase ) const final;

  pvt::ScalarPropertyAndDerivatives< double > const & getPhaseMoleFraction( pvt::PHASE_TYPE const & phase ) const final;

  double const & getPressure() const;

  const std::vector< double > & getFeed() const;

  void setPressure( double const & pressure );

  virtual void setFeed( std::vector< double > const & feed );

  const std::vector< pvt::PHASE_TYPE > & getPhases() const;

  void setPhaseMoleFractionDP( pvt::PHASE_TYPE const & phase,
                               double const & value );

  void setMolecularWeightDP( pvt::PHASE_TYPE const & phase,
                             double const & value );

  void setMoleDensityDP( pvt::PHASE_TYPE const & phase,
                         double const & value );

  void setMassDensityDP( pvt::PHASE_TYPE const & phase,
                         double const & value );

  void setViscosityDP( pvt::PHASE_TYPE const & phase,
		       double const & value );
  
  void setMoleCompositionDP( pvt::PHASE_TYPE const & phase,
                             std::vector< double > const & value );

  void setPhaseMoleFractionDZ( pvt::PHASE_TYPE const & phase,
                               std::size_t i,
                               double const & value );

  void setMolecularWeightDZ( pvt::PHASE_TYPE const & phase,
                             std::size_t i,
                             double const & value );

  void setMoleDensityDZ( pvt::PHASE_TYPE const & phase,
                         std::size_t i,
                         double const & value );

  void setMassDensityDZ( pvt::PHASE_TYPE const & phase,
                         std::size_t i,
                         double const & value );

  void setViscosityDZ( pvt::PHASE_TYPE const & phase,
		       std::size_t i,
		       double const & value );
  
  void setMoleCompositionDZ( pvt::PHASE_TYPE const & phase,
                             std::size_t i,
                             std::vector< double > const & value );

protected:

  std::map< pvt::PHASE_TYPE, pvt::ScalarPropertyAndDerivatives< double > > m_massDensity;
  std::map< pvt::PHASE_TYPE, pvt::VectorPropertyAndDerivatives< double > > m_moleComposition;
  std::map< pvt::PHASE_TYPE, pvt::ScalarPropertyAndDerivatives< double > > m_moleDensity;
  std::map< pvt::PHASE_TYPE, pvt::ScalarPropertyAndDerivatives< double > > m_viscosity;  
  std::map< pvt::PHASE_TYPE, pvt::ScalarPropertyAndDerivatives< double > > m_molecularWeight;
  std::map< pvt::PHASE_TYPE, pvt::ScalarPropertyAndDerivatives< double > > m_phaseMoleFraction;
  
  void setPhaseMoleFraction( pvt::PHASE_TYPE const & phase,
                             const double & phaseMoleFraction );

  void setMoleComposition( pvt::PHASE_TYPE const & phase,
                           const std::vector< double > & moleComposition );

private:

  std::vector< pvt::PHASE_TYPE > m_phases;
  double m_pressure;
  std::vector< double > m_feed;
};

}

#endif //PVTPACKAGE_FACTORMULTIPHASESYSTEMPROPERTIES_HPP
