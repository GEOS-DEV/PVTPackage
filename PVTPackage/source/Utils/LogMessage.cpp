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

// Header include
#include "Utils/LogMessage.hpp"

namespace PVTPackage
{

LogMessage * LogMessage::instance()
{
  return instance( VerbosityLevelLogFile::INFO );
}

LogMessage * LogMessage::instance( VerbosityLevelLogFile level )
{
  static LogMessage s_instance( level );
  return &s_instance;
}

LogMessage::LogMessage()
{
  m_messageLevel = VerbosityLevelLogFile::INFO;
  m_message << "";
}

LogMessage::LogMessage( VerbosityLevelLogFile level )
{
  m_messageLevel = level;
  m_message << "";
}

}
