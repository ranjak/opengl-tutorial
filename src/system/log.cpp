#include "log.hpp"


Log::Log() :
  Log(std::cout)
{
}


Log::Log(std::ostream& output) :
  mOutput(output),
  mLevel(Priority::DBG)
{
}


std::string Log::priorityToStr(Log::Priority sev)
{
  std::string priorityStr;
  switch (sev) {
  case Priority::ERROR:
    priorityStr = "[ERROR] ";
    break;
  case Priority::WARNING:
    priorityStr = "[WARNING] ";
    break;
  case Priority::INFO:
    priorityStr = "[INFO] ";
    break;
  case Priority::DBG:
    priorityStr = "[DEBUG] ";
    break;
  case Priority::TRACE:
    priorityStr = "[TRACE] ";
    break;
  default:
    priorityStr = "";
    break;
  }

  return priorityStr;
}

Log& Log::getGlobal()
{
  static Log globalLogger;

  return globalLogger;
}


void Log::log(Priority sev, const std::string& msg)
{
  if (sev <= mLevel) {
    std::string priorityStr = priorityToStr(sev);

    mOutput << priorityStr << msg << std::endl;
  }
}

void Log::log(Priority sev, std::stringstream &msg)
{
  if (sev <= mLevel) {
    mOutput << priorityToStr(sev) << msg.str() << std::endl;
  }
}


void Log::setLevel(Priority maxLevel)
{
  this->mLevel = maxLevel;
}

std::ostream &Log::get(Log::Priority sev)
{
  mOutput << priorityToStr(sev);
  return mOutput;
}


void Log::error(std::string msg)
{
  Log::getGlobal().log(Log::Priority::ERROR, msg);
}


