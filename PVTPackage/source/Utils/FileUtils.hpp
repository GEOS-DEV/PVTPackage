/*
 * ------------------------------------------------------------------------------------------------------------
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 * Copyright (c) 2016-2024 Lawrence Livermore National Security LLC
 * Copyright (c) 2018-2024 Total, S.A
 * Copyright (c) 2018-2024 The Board of Trustees of the Leland Stanford Junior University
 * Copyright (c) 2018-2024 Chevron 
 * Copyright (c) 2019-     GEOS/GEOSX Contributors
 * All rights reserved
 *
 * See top level LICENSE, COPYRIGHT, CONTRIBUTORS, NOTICE, and ACKNOWLEDGEMENTS files for details.
 * ------------------------------------------------------------------------------------------------------------
 */

#pragma once

#ifndef PVTPACKAGE_FILEUTILS_HPP
#define PVTPACKAGE_FILEUTILS_HPP

#include "StringUtils.hpp"
#include "Assert.hpp"

#include <string>
#include <vector>
#include <fstream>

namespace PVTPackage
{

namespace FileUtils
{

template< typename T >
void ReadTable( const std::string & fileName,
                std::vector< std::vector< T > > & data )
{
  std::ifstream is( fileName );
  ASSERT( is.is_open(), "Could not open file: " + fileName );

  constexpr std::streamsize buf_size = 256;
  char buf[buf_size];

  // Read line-by-line until eof
  while( is.getline( buf, buf_size ) )
  {
    std::string str( buf );

    // Remove whitespace and end-of-line characters, if any
    StringUtils::Trim( str );

    // Remove # and -- (Eclipse-style) comments
    StringUtils::RemoveStringAndFollowingContentFromLine( "#", str );
    StringUtils::RemoveStringAndFollowingContentFromLine( "--", str );

    // Skip empty or comment-only strings
    if( str.empty() )
    {
      continue;
    }

    // Add and read a new line entry
    data.emplace_back( 0 );
    StringUtils::FromStringTo( str, data.back() );

    // Remove line entry of no data read
    if( data.back().empty() )
    {
      data.pop_back();
    }
  }

  is.close();
}

}

} // namespace PVTPackage

#endif //PVTPACKAGE_FILEUTILS_HPP
