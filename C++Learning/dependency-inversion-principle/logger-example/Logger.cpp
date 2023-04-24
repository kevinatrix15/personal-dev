#include "Logger.h"

using namespace standard;

Logger::Logger(std::string_view logFilename)
{
  output_stream_.open(logFilename.data(), ios_base::app);
  if (!output_stream_.good()) {
    throw std::runtime_error {"Unable to initialize the logger"};
  }
}

Logger::~Logger()
{
  output_stream_ << "Logger shutting down." << std::endl;
  output_stream_.close();
}

// publie methods

void Logger::setLogLevel(LogLevel level)
{
  log_level_ = level;
}

void Logger::log(std::string_view message, LogLevel logLevel)
{
  if (log_level_ < logLevel) {return;}

  output_stream_ << format("{}: {}", getLogLevelString(logLevel), message) << std::endl;
}


// private methods

std::string_view Logger::getLogLevelString(LogLevel level) const
{
  switch (level) {
    case LogLevel::Error: return "ERROR";
    case LogLevel::Info: return "INFO";
    case LogLevel::Debug: return "DEBUG";
  }
  throw std::runtime_error {"Invalid log level."};
}

