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

// Project includes
#include "Utils/LogMessage.hpp"

// Std library includes
#include <fstream>
#include <chrono>
#include <iostream>
#include <iomanip>

////Macros
#define LOGWARNING( msg ) do {PVTPackage::Logger::instance()->LogWARNING(msg);} while (0)
#define LOGERROR( msg ) do {PVTPackage::Logger::instance()->LogERROR(msg);} while (0)
#define LOGINFO( msg ) do {PVTPackage::Logger::instance()->LogINFO(msg);} while (0)
#define LOGDEBUG( msg ) do {PVTPackage::Logger::instance()->LogDEBUG(msg);} while (0)

//ADGPRS Native style
#define LogFatal( msg ) do {PVTPackage::Logger::instance()->Log((*(PVTPackage::LogMessage::instance(VerbosityLevelLogFile::ERROR))<<msg));} while (0)
#define LogMessage2( lvl ) do {PVTPackage::*(PVTPackage::LogMessage::instance(VerbosityLevelLogFile::lvl));} while (0)

namespace PVTPackage
{

enum class VerbosityLevelScreen
{
  SILENT, BRIEF, ALL
};

class Logger
{

public:

  static Logger * instance();

  static void init( std::string LevelLogFile,
                    std::string fileName,
                    std::string LevelScreen );

  [[noreturn]] void LogERROR( std::string msg );

  void LogWARNING( std::string msg );

  void LogINFO( std::string msg );

  void LogDEBUG( std::string msg );

  void Log( LogMessage & msg );

protected:

  Logger( std::string levelLogfile,
          std::string fileName,
          std::string levelScreen );

private:

  //Screen
  VerbosityLevelScreen m_screenLevel;

  //Log file
  VerbosityLevelLogFile m_logLevel;

  std::ofstream m_logfile;

  template< typename T >
  static void write_screen( T && t );

  template< typename Head, typename Tail, typename... Tails >
  static void write_screen( Head && head,
                            Tail && tail,
                            Tails && ... tails );

  template< typename T >
  void write_file( T && t );

  template< typename Head, typename Tail, typename... Tails >
  void write_file( Head && head,
                   Tail && tail,
                   Tails && ... tails );

  static std::string get_time();

};


template< typename T >
void Logger::write_screen( T && t )
{
  std::cout << t << std::endl;
}

template< typename Head, typename Tail, typename... Tails >
void Logger::write_screen( Head && head,
                           Tail && tail,
                           Tails && ... tails )
{
  std::cout << head;
  write_screen( std::forward< Tail >( tail ), std::forward< Tails >( tails )... );
}

template< typename T >
void Logger::write_file( T && t )
{
  m_logfile << t << std::endl;
}

template< typename Head, typename Tail, typename... Tails >
void Logger::write_file( Head && head,
                         Tail && tail,
                         Tails && ... tails )
{
  m_logfile << head;
  write_file( std::forward< Tail >( tail ), std::forward< Tails >( tails )... );
}

}
