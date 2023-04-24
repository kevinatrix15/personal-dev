#include <fstream>
#include <string_view>

class ILogger
{
  public:
    virtual ~ILogger() = default;

    enum class LogLevel {
      Error,
      Info,
      Debug
    };

    virtual void setLogLevel(LogLevel level) = 0;
    virtual void log(std::string_view message, LogLevel logLevel) = 0;
};

class Logger : public ILogger
{
  public:
    explicit Logger(std::string_view logFilename);
    virtual ~Logger();
    void setLogLevel(LogLevel level) override;
    void log(std::string_view message, LogLevel logLevel) override;

  private:
    // converts a log level to a human readable string
    std::string_view getLogLevelString(LogLevel level) const;

    std::ofstream output_stream_;
    LogLevel log_level_;
};
