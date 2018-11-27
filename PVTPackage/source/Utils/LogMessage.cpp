// Header include
#include "Utils/LogMessage.hpp"

namespace PVTPackage {
LogMessage* LogMessage::instance()
{
  return instance(VerbosityLevelLogFile::INFO);
}

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wexit-time-destructors"
#pragma clang diagnostic ignored "-Wglobal-constructors"
#endif

LogMessage* LogMessage::instance(VerbosityLevelLogFile lvl)
{
  static LogMessage s_instance(lvl);
  return &s_instance;
}

#ifdef __clang__
#pragma clang diagnostic pop
#endif

LogMessage::LogMessage()
{
  m_message_level = VerbosityLevelLogFile::INFO;
  m_message << "";
}

LogMessage::LogMessage(VerbosityLevelLogFile lvl)
{
  m_message_level = lvl;
  m_message << "";
}
}
