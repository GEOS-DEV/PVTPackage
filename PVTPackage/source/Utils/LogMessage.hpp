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

#include <sstream>

namespace PVTPackage
{

enum class VerbosityLevelLogFile
{
  ERROR, WARNING, DEBUG, INFO
};

class LogMessage
{

public:

  static LogMessage * instance();

  static LogMessage * instance( VerbosityLevelLogFile level );

  std::string getMessage() const
  { return m_message.str(); }

  VerbosityLevelLogFile & getMessageLevel()
  { return m_messageLevel; }


  template< typename T >
  LogMessage & operator<<( const T & msg )
  {
    m_message << msg;
    return *this;
  }

private:

  VerbosityLevelLogFile m_messageLevel;

  std::stringstream m_message;

  LogMessage();

  explicit LogMessage( VerbosityLevelLogFile level );


};
} // namespace
