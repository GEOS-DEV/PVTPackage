#pragma once

// Project includes
#include "Utils/LogMessage.hpp"

// Std library includes
#include <fstream>
#include <chrono>
#include <iostream>
#include <iomanip>

////Macros
#define LOGWARNING(msg) do {PVTPackage::Logger::instance()->LogWARNING(msg);} while (0)
#define LOGERROR(msg) do {PVTPackage::Logger::instance()->LogERROR(msg);} while (0)
#define LOGINFO(msg) do {PVTPackage::Logger::instance()->LogINFO(msg);} while (0)
#define LOGDEBUG(msg) do {PVTPackage::Logger::instance()->LogDEBUG(msg);} while (0)

//ADGPRS Native style
#define LogFatal(msg) do {PVTPackage::Logger::instance()->Log((*(PVTPackage::LogMessage::instance(VerbosityLevelLogFile::ERROR))<<msg));} while (0)
#define LogMessage2(lvl) do {PVTPackage::*(PVTPackage::LogMessage::instance(VerbosityLevelLogFile::lvl));} while (0)

namespace PVTPackage {

enum class VerbosityLevelScreen
{
  SILENT, BRIEF, ALL
};

class Logger
{

public:

  static Logger* instance();
  static void init(std::string LevelLogFile, std::string file_name, std::string LevelScreen);

  void LogERROR(std::string msg);
  void LogWARNING(std::string msg);
  void LogINFO(std::string msg);
  void LogDEBUG(std::string msg);

  void Log(LogMessage& msg);

protected:

  Logger(std::string level_logfile, std::string file_name, std::string level_screen);

  //Input
  static std::string m_level_logfile;
  static std::string m_file_name;
  static std::string m_level_screen;

private:

  //Screen
  VerbosityLevelScreen m_screen_level;

  //Log file
  VerbosityLevelLogFile m_log_level;

  std::ofstream m_logfile;

  template<typename T>
  static void write_screen(T &&t);

  template<typename Head, typename Tail, typename... Tails>
  static void write_screen(Head &&head, Tail &&tail, Tails&&... tails);

  template<typename T>
  void write_file(T &&t);

  template<typename Head, typename Tail, typename... Tails>
  void write_file(Head &&head, Tail &&tail, Tails&&... tails);

  static std::string get_time();

};


template<typename T>
void Logger::write_screen(T &&t)
{
  std::cout << t << std::endl;
}

template<typename Head, typename Tail, typename... Tails>
void Logger::write_screen(Head &&head, Tail &&tail, Tails&&... tails)
{
  std::cout << head;
  write_screen(std::forward<Tail>(tail), std::forward<Tails>(tails)...);
}

template<typename T>
void Logger::write_file(T &&t)
{
  m_logfile << t << std::endl;
}

template<typename Head, typename Tail, typename... Tails>
void Logger::write_file(Head &&head, Tail &&tail, Tails&&... tails)
{
  m_logfile << head;
  write_file(std::forward<Tail>(tail), std::forward<Tails>(tails)...);
}
}
