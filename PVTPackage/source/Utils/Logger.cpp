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

#include "Logger.hpp"

#include <iomanip>

#define PVT_LOG_LEVEL_FILE "DEBUG"
#define PVT_LOG_LEVEL_SCREEN "ALL"
#define PVT_LOG_FILE_NAME "pvt.log"

namespace PVTPackage
{

Logger * Logger::instance()
{
  static Logger s_instance( PVT_LOG_LEVEL_FILE, PVT_LOG_FILE_NAME, PVT_LOG_LEVEL_SCREEN );
  return &s_instance;
}

Logger::Logger( std::string levelLogfile,
                std::string fileName,
                std::string levelScreen )
{

  //Log file
  if( levelLogfile == "ERROR" )
  {
    m_logLevel = VerbosityLevelLogFile::ERROR;
  }
  if( levelLogfile == "WARNING" )
  {
    m_logLevel = VerbosityLevelLogFile::WARNING;
  }
  if( levelLogfile == "INFO" )
  {
    m_logLevel = VerbosityLevelLogFile::INFO;
  }
  if( levelLogfile == "DEBUG" )
  {
    m_logLevel = VerbosityLevelLogFile::DEBUG;
  }

  m_logfile.open( fileName );

  //Screen
  if( levelScreen == "SILENT" )
  {
    m_screenLevel = VerbosityLevelScreen::SILENT;
  }
  if( levelScreen == "BRIEF" )
  {
    m_screenLevel = VerbosityLevelScreen::BRIEF;
  }
  if( levelScreen == "ALL" )
  {
    m_screenLevel = VerbosityLevelScreen::ALL;
  }

}

void Logger::LogERROR( std::string msg )
{

  std::string message = "[" + get_time() + "] ERROR:   " + msg.c_str();
  write_file( message );
  if( m_screenLevel >= VerbosityLevelScreen::BRIEF )
  {
    write_screen( "ERROR:   " + msg + " in file " + __FILE__ + " line " + std::to_string( __LINE__ ) );
  }
  abort();
}

void Logger::LogWARNING( std::string msg )
{

  std::string message = "[" + get_time() + "] WARNING: " + msg.c_str();
  if( m_logLevel >= VerbosityLevelLogFile::WARNING )
  {
    write_file( message );
  }
  if( m_screenLevel >= VerbosityLevelScreen::BRIEF )
  {
    write_screen( "WARNING: " + msg );
  }

}

void Logger::LogINFO( std::string msg )
{

  std::string message = "[" + get_time() + "] INFO:    " + msg.c_str();
  if( m_logLevel >= VerbosityLevelLogFile::INFO )
  {
    write_file( message );
  }
  if( m_screenLevel >= VerbosityLevelScreen::BRIEF )
  {
    write_screen( msg );
  }
}

void Logger::LogDEBUG( std::string msg )
{
  std::string message = "[" + get_time() + "] DEBUG:   " + msg.c_str();
  if( m_logLevel >= VerbosityLevelLogFile::DEBUG )
  {
    write_file( message );
  }
  if( m_screenLevel >= VerbosityLevelScreen::ALL )
  {
    write_screen( msg );
  }
}

void Logger::Log( LogMessage & msg )
{
  std::string str_msg = msg.getMessage();
  VerbosityLevelLogFile MessageLevel = msg.getMessageLevel();
  if( MessageLevel == VerbosityLevelLogFile::ERROR )
  {
    LogERROR( str_msg );
  }
  else if( MessageLevel == VerbosityLevelLogFile::WARNING )
  {
    LogWARNING( str_msg );
  }
  else if( MessageLevel == VerbosityLevelLogFile::DEBUG )
  {
    LogDEBUG( str_msg );
  }
  else if( MessageLevel == VerbosityLevelLogFile::INFO )
  {
    LogINFO( str_msg );
  }
}

std::string Logger::get_time()
{
  std::stringstream res;
  time_t t = time( nullptr );
  struct tm * now;
#ifdef WIN32
  struct tm now_st;
  now = &now_st;
  localtime_s(now, &t);
#else
  now = localtime( &t );
#endif
//  res << std::put_time(now, "%Y-%m-%d %H:%M:%S");
  res << ( now->tm_year + 1900 ) << "-" << ( now->tm_mon + 1 ) << "-" << ( now->tm_mday ) << "-" << now->tm_hour << ":" << now->tm_min << ":" << now->tm_sec;
  return res.str();
}
}
