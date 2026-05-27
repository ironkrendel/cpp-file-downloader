#ifndef CPP_FILE_DOWNLOADER_LOGGER_H
#define CPP_FILE_DOWNLOADER_LOGGER_H

#include <string>

namespace cfd
{
    namespace logger
    {
        void Log(const std::string& message);
        void LogError(const std::string& error_message);
    };
}

#endif //CPP_FILE_DOWNLOADER_LOGGER_H
