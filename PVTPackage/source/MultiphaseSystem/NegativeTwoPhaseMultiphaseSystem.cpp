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

#include "NegativeTwoPhaseMultiphaseSystem.hpp"

#include "MultiphaseSystem/ComponentProperties.hpp"

#include "pvt/pvt.hpp"

#include <memory>

namespace PVTPackage
{

std::unique_ptr< NegativeTwoPhaseMultiphaseSystem > NegativeTwoPhaseMultiphaseSystem::build( const std::vector< pvt::PHASE_TYPE > & phases,
                                                                                             const std::vector< pvt::EOS_TYPE > & eosTypes,
                                                                                             const std::vector< std::string > & componentNames,
                                                                                             const std::vector< double > & componentMolarWeights,
                                                                                             const std::vector< double > & componentCriticalTemperatures,
                                                                                             const std::vector< double > & componentCriticalPressures,
                                                                                             const std::vector< double > & componentOmegas )
{
  if( not areComponentDataConsistent( componentNames,
                                      componentMolarWeights,
                                      componentCriticalTemperatures,
                                      componentCriticalPressures,
                                      componentOmegas ) )
  {
    return std::unique_ptr< NegativeTwoPhaseMultiphaseSystem >();
  }

  ComponentProperties cp( componentNames.size(),
                          componentNames,
                          componentMolarWeights,
                          componentCriticalTemperatures,
                          componentCriticalPressures,
                          componentOmegas );

  // I am not using std::make_unique because I want the constructor to be private.
  auto * ptr = new NegativeTwoPhaseMultiphaseSystem( phases, eosTypes, cp );
  return std::unique_ptr< NegativeTwoPhaseMultiphaseSystem >( ptr );
}

NegativeTwoPhaseMultiphaseSystem::NegativeTwoPhaseMultiphaseSystem( const std::vector< pvt::PHASE_TYPE > & phases,
                                                                    const std::vector< pvt::EOS_TYPE > & eosTypes,
                                                                    const ComponentProperties & componentProperties )
  :
  m_negativeTwoPhaseFlash( phases, eosTypes, componentProperties ),
  m_ntpfmsp( phases, componentProperties.NComponents )
{

}

void NegativeTwoPhaseMultiphaseSystem::Update( double pressure,
                                               double temperature,
                                               std::vector< double > feed )
{
  m_ntpfmsp.setTemperature( temperature );
  m_ntpfmsp.setPressure( pressure );
  m_ntpfmsp.setFeed( feed );

  const bool result = computeEquilibriumAndDerivativesWithTemperature( m_negativeTwoPhaseFlash, m_ntpfmsp );

  m_stateIndicator = result ? State::SUCCESS : State::NOT_CONVERGED;
}

const pvt::MultiphaseSystemProperties & NegativeTwoPhaseMultiphaseSystem::getMultiphaseSystemProperties() const
{
  return m_ntpfmsp;
}

#if defined(GEOSX_USE_OPENMP)
void NegativeTwoPhaseMultiphaseSystem::BatchUpdate( std::vector< double > const & pressure,
                                                    std::vector< double > const & temperature,
                                                    std::vector< std::vector< double > > const & feed )
{
  const std::size_t n = pressure.size();
  ASSERT( temperature.size() == n, "Inconsistent inputs" );
  ASSERT( feed.size() == n, "Inconsistent inputs" );

  m_batchProps.reserve( n );
  for( std::size_t i = 0; i < n; ++i )
  {
    NegativeTwoPhaseFlashMultiphaseSystemProperties d( m_ntpfmsp.getPhases(), feed[i].size() );// FIXME note sure about the feed.size()...
    d.setPressure( pressure[i] );
    d.setTemperature( temperature[i] );
    d.setFeed( feed[i] );
    m_batchProps.emplace_back( d );
  }

  auto t_start = std::chrono::high_resolution_clock::now();
  #pragma omp parallel for default(none)
  for( std::size_t i = 0; i < n; ++i )
  {
//    std::cout << "max:" << omp_get_max_threads() << std::endl;
//    std::cout << omp_get_thread_num() << std::endl;
//    m_negativeTwoPhaseFlash.computeEquilibrium( m_batchProps[i] );
    computeEquilibriumAndDerivativesWithTemperature( m_negativeTwoPhaseFlash, m_batchProps[i] );
  }
// do something
  auto t_end = std::chrono::high_resolution_clock::now();
  double elapsed_time_ms = std::chrono::duration< double, std::milli >( t_end - t_start ).count();
  std::cout << "TIME (ms) = " << elapsed_time_ms << std::endl;
}
#endif
}
