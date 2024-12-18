/*	
 * ------------------------------------------------------------------------------------------------------------	
 * SPDX-License-Identifier: LGPL-2.1-only	
 *	
 * Copyright (c) 2018-2024 Lawrence Livermore National Security LLC	
 * Copyright (c) 2018-2024 The Board of Trustees of the Leland Stanford Junior University	
 * Copyright (c) 2018-2024 TotalEnergies	
 * Copyright (c) 2019-     GEOS/GEOSX Contributors	
 * All right reserved	
 *	
 * See top level LICENSE, COPYRIGHT, CONTRIBUTORS, NOTICE, and ACKNOWLEDGEMENTS files for details.	
 * ------------------------------------------------------------------------------------------------------------	
 */

#ifndef GEOS_SERIALIZERS_MULTIPHASESYSTEMPROPERTIES_HPP
#define GEOS_SERIALIZERS_MULTIPHASESYSTEMPROPERTIES_HPP

#include "pvt/pvt.hpp"

#include <nlohmann/json.hpp>

namespace pvt {

void to_json( nlohmann::json & dataProvider, const MultiphaseSystemProperties & msp);

}

#endif //GEOS_SERIALIZERS_MULTIPHASESYSTEMPROPERTIES_HPP
