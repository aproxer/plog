#pragma once
#include <cstring>
#include <plog/Logger.h>
#include <plog/Formatters/CsvFormatter.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Appenders/FileAppender.h>
#include <plog/Appenders/RollingFileAppender.h>

namespace plog
{
    namespace
    {
        bool isCsv(const char* fileName)
        {
            const char kCsvExt[] = ".csv";
            const size_t kCsvExtLength = sizeof(kCsvExt) - 1;

            size_t len = std::strlen(fileName);
            return len >= kCsvExtLength && 0 == std::strcmp(&fileName[len - kCsvExtLength], kCsvExt);
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // Empty initializer / one appender

    template<int instance>
    inline Logger<instance>& init(Severity maxSeverity = none, IAppender* appender = NULL)
    {
        static Logger<instance> logger(maxSeverity);
        return appender ? logger.addAppender(appender) : logger;
    }

    inline Logger<0>& init(Severity maxSeverity = none, IAppender* appender = NULL)
    {
        return init<0>(maxSeverity, appender);
    }

    //////////////////////////////////////////////////////////////////////////
    // File/RollingFile with any Formatter

    template<int instance, class Formatter>
    inline Logger<instance>& init(Severity maxSeverity, const char* fileName)
    {
        static FileAppender<Formatter> fileAppender(fileName);
        return init<instance>(maxSeverity, &fileAppender);
    }

    template<int instance, class Formatter>
    inline Logger<instance>& init(Severity maxSeverity, const char* fileName, size_t maxFileSize, int maxFiles)
    {
        static RollingFileAppender<Formatter> rollingFileAppender(fileName, maxFileSize, maxFiles);
        return init<instance>(maxSeverity, &rollingFileAppender);
    }

    //////////////////////////////////////////////////////////////////////////
    // File/RollingFile with CSV

    template<int instance>
    inline Logger<instance>& initCsv(Severity maxSeverity, const char* fileName, size_t maxFileSize = 0, int maxFiles = 0)
    {
        return maxFileSize ? init<instance, CsvFormatter>(maxSeverity, fileName, maxFileSize, maxFiles) : init<instance, CsvFormatter>(maxSeverity, fileName);
    }

    //////////////////////////////////////////////////////////////////////////
    // File/RollingFile with TXT

    template<int instance>
    inline Logger<instance>& initTxt(Severity maxSeverity, const char* fileName, size_t maxFileSize = 0, int maxFiles = 0)
    {
        return maxFileSize ? init<instance, TxtFormatter>(maxSeverity, fileName, maxFileSize, maxFiles) : init<instance, TxtFormatter>(maxSeverity, fileName);
    }
    
    //////////////////////////////////////////////////////////////////////////
    // File/RollingFile with TXT/CSV chosen by file extension

    template<int instance>
    inline Logger<instance>& init(Severity maxSeverity, const char* fileName, size_t maxFileSize = 0, int maxFiles = 0)
    {
        return isCsv(fileName) ? initCsv<instance>(maxSeverity, fileName, maxFileSize, maxFiles) : initTxt<instance>(maxSeverity, fileName, maxFileSize, maxFiles);
    }

    inline Logger<0>& init(Severity maxSeverity, const char* fileName, size_t maxFileSize = 0, int maxFiles = 0)
    {
        return init<0>(maxSeverity, fileName, maxFileSize, maxFiles);
    }

    //////////////////////////////////////////////////////////////////////////
    // WCHAR variants for Windows

#ifdef _WIN32
    template<int instance>
    inline Logger<instance>& initCsv(Severity maxSeverity, const wchar_t* fileName, size_t maxFileSize = 0, int maxFiles = 0)
    {
        return init<instance, CsvFormatter>(maxSeverity, util::toString(fileName).c_str(), maxFileSize, maxFiles);
    }

    template<int instance>
    inline Logger<instance>& initTxt(Severity maxSeverity, const wchar_t* fileName, size_t maxFileSize = 0, int maxFiles = 0)
    {
        return init<instance, TxtFormatter>(maxSeverity, util::toString(fileName).c_str(), maxFileSize, maxFiles);
    }

    template<int instance>
    inline Logger<instance>& init(Severity maxSeverity, const wchar_t* fileName, size_t maxFileSize = 0, int maxFiles = 0)
    {
        return init<instance>(maxSeverity, util::toString(fileName).c_str(), maxFileSize, maxFiles);
    }

    inline Logger<0>& init(Severity maxSeverity, const wchar_t* fileName, size_t maxFileSize = 0, int maxFiles = 0)
    {
        return init<0>(maxSeverity, util::toString(fileName).c_str(), maxFileSize, maxFiles);
    }
#endif
}
