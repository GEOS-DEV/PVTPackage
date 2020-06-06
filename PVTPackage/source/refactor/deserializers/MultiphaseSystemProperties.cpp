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

#include "refactor/deserializers/MultiphaseSystemProperties.hpp"

#include "refactor/JsonKeys.hpp"

#include "refactor/passiveDataStructures/MultiphaseSystemProperties.hpp"

#include "refactor/deserializers/PVTEnums.hpp"
#include "refactor/deserializers/PropertyAndDerivatives.hpp"
#include "refactor/deserializers/PhaseProperties.hpp"
#include "refactor/deserializers/PhaseModels.hpp"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace PVTPackage
{
namespace refactor
{
namespace pds
{

void from_json( const json & j,
                pds::MultiphaseSystemProperties & props )
{
  j.at(MultiphaseSystemPropertiesKeys::TEMPERATURE).get_to(props.Temperature);
  j.at(MultiphaseSystemPropertiesKeys::PRESSURE).get_to(props.Pressure);
  j.at(MultiphaseSystemPropertiesKeys::FEED).get_to(props.Feed);

  j.at(MultiphaseSystemPropertiesKeys::PHASE_STATE).get_to(props.PhaseState);
  j.at(MultiphaseSystemPropertiesKeys::PHASE_TYPES).get_to(props.PhaseTypes);

  for( const pds::PHASE_TYPE & pt: props.PhaseTypes )
  {
    // Small hack to put the enum as a key
    std::string const & ptKey = json( pt ).get< std::string >();

    // TODO REFACTOR check this cast
    auto phaseModelAtPt = j[MultiphaseSystemPropertiesKeys::PHASE_MODELS][ptKey].get< std::shared_ptr< pds::PhaseModel > >();
    props.PhaseModels[ pt ] = phaseModelAtPt;
    props.PhaseMoleFraction[ pt ] = j[MultiphaseSystemPropertiesKeys::PHASE_MOLE_FRACTION][ptKey];
    props.PhasesProperties[ pt ] = j[MultiphaseSystemPropertiesKeys::PHASE_PROPERTIES][ptKey];
  }
}

} // end of namespace pds
} // end of namespace refactor
} // end of namespace PVTPackage
