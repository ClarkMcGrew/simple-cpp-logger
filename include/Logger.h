//
// Created by Nadrino on 24/08/2020.
//

#ifndef SIMPLE_CPP_LOGGER_LOGGER_H
#define SIMPLE_CPP_LOGGER_LOGGER_H


#include <string>
#include <mutex>
#include <vector>

#include "implementation/LoggerUtils.h"
#include "implementation/LoggerParameters.h"


// Here is what you want to use
#define LogFatal       (Logger{Logger::LogLevel::FATAL,    __FILENAME__, __LINE__})
#define LogError       (Logger{Logger::LogLevel::ERROR,    __FILENAME__, __LINE__})
#define LogAlert       (Logger{Logger::LogLevel::ALERT,    __FILENAME__, __LINE__})
#define LogWarning     (Logger{Logger::LogLevel::WARNING,  __FILENAME__, __LINE__})
#define LogInfo        (Logger{Logger::LogLevel::INFO,     __FILENAME__, __LINE__})
#define LogDebug       (Logger{Logger::LogLevel::DEBUG,    __FILENAME__, __LINE__})
#define LogTrace       (Logger{Logger::LogLevel::TRACE,    __FILENAME__, __LINE__})


namespace {

  class Logger {

  public:

    enum class LogLevel {
      FATAL   = 0,
      ERROR   = 1,
      ALERT   = 2,
      WARNING = 3,
      INFO    = 4,
      DEBUG   = 5,
      TRACE   = 6
    };
    enum class PrefixLevel {
      NONE        = 0,
      MINIMAL     = 1,
      PRODUCTION  = 2,
      DEBUG       = 3,
      FULL        = 4
    };

    //! Setters
    // Keep in mind that every parameter you set will be applied only in the context of the source file you're in
    // It is an inherent feature as a **header-only** library
    static void setMaxLogLevel(int maxLogLevel_);
    static void setMaxLogLevel(const LogLevel &maxLogLevel_);
    static void setEnableColors(bool enableColors_);
    static void setPropagateColorsOnUserHeader(bool propagateColorsOnUserHeader_);
    static void setPrefixLevel(const PrefixLevel &prefixLevel_);
    static void setUserHeaderStr(const std::string &userHeaderStr_);
    static void setPrefixFormat(const std::string &prefixFormat_);

    //! Getters
    static int getMaxLogLevelInt();
    static const LogLevel & getMaxLogLevel();
    static std::string getPrefixString();                                // LogWarning.getPrefixString()
    static std::string getPrefixString(const Logger& loggerConstructor); // Logger::getPrefixString(LogWarning)

    //! Misc
    static void quietLineJump();

    //! Non-static Methods
    // For printf-style calls
    template <typename... TT> void operator()(const char *fmt_str, TT && ... args);
    // For std::cout-style calls
    template<typename T> Logger &operator<<(const T &data);
    Logger &operator<<(std::ostream &(*f)(std::ostream &));


    // Macro-Related Methods
    // Those intended to be called using the above preprocessor macros
    Logger(const LogLevel &logLevel_, char const * fileName_, const int &lineNumber_);
    virtual ~Logger();

  protected:

    static void buildCurrentPrefix();
    static void formatUserHeaderStr(std::string &strBuffer_);

    static void hookStreamBuffer();
    static std::string getLogLevelColorStr(const LogLevel &selectedLogLevel_);
    static std::string getLogLevelStr(const LogLevel &selectedLogLevel_);

    template<typename ... Args> static void printFormat(const char *fmt_str, Args ... args );


  private:

    // parameters
    static bool _enableColors_;
    static bool _disablePrintfLineJump_;
    static bool _propagateColorsOnUserHeader_;
    static bool _cleanLineBeforePrint_;
    static LogLevel _maxLogLevel_;
    static PrefixLevel _prefixLevel_;
    static std::string _userHeaderStr_;
    static std::string _prefixFormat_;

    // internal
    static LogLevel _currentLogLevel_;
    static std::string _currentFileName_;
    static int _currentLineNumber_;
    static std::string _currentPrefix_;
    static bool _isNewLine_;
    static std::ostream& _outputStream_;
    static std::mutex _loggerMutex_;
    static LoggerUtils::LastCharBuffer* _lastCharKeeper_;

  };

}

#include "implementation/Logger.impl.h"

#endif //SIMPLE_CPP_LOGGER_LOGGER_H
