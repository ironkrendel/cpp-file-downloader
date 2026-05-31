#include "cfd/Logger.h"
#include "cfd/URLParser.h"

#include <fstream>
#include <vector>
#include <filesystem>

namespace logger = cfd::logger;
namespace link_parser = cfd::URLParser;

// ARGS
// 1) File that contains the URL list
// 2) Target directory
// 3) Max concurrent downloads count
int main(const int argc, const char * argv[])
{
    if (argc != 4)
    {
        logger::LogError("Wrong number of arguments");

        return 1;
    }

    logger::Log("C++ File Downloader starting");

    const std::string input_args_string = std::string(argv[1]) + " " + std::string(argv[2]) + " " + std::string(argv[3]);
    logger::Log(input_args_string);

    logger::Log("Reading URL list");

    std::ifstream url_file(argv[1]);
    std::vector<link_parser::URLTarget> urls;
    std::string url_file_line_buffer;
    while (std::getline(url_file, url_file_line_buffer))
    {
        if (url_file_line_buffer.length() != 0)
        {
            logger::Log(url_file_line_buffer);
            urls.push_back(link_parser::ParseURL(url_file_line_buffer));
        }
    }

    if (!std::filesystem::is_directory(argv[2]))
    {
        std::filesystem::create_directories(argv[2]);
    }

    return 0;
}