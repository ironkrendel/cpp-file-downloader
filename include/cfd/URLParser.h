#ifndef CPP_FILE_DOWNLOADER_URLPARSER_H
#define CPP_FILE_DOWNLOADER_URLPARSER_H

#include <string>

namespace cfd
{
    namespace URLParser
    {
        struct URLTarget
        {
            std::string host;
            std::string target;
            std::string port;
            std::string filename;
            std::string extension;
        };

        std::pair<std::string, std::string> SplitFilename(const std::string& filename);
        URLTarget ParseURL(const std::string& url);
    }
}

#endif //CPP_FILE_DOWNLOADER_URLPARSER_H
