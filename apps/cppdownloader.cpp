#include "cfd/Logger.h"
#include "cfd/URLParser.h"
#include "cfd/File.h"

#include <fstream>
#include <vector>
#include <filesystem>

namespace logger = cfd::logger;
namespace link_parser = cfd::URLParser;
namespace file = cfd::file;

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

    const std::size_t max_concurrent_files = std::stoul(argv[3]);

    const std::string input_args_string = std::string(argv[1]) + " " + std::string(argv[2]) + " " + std::string(argv[3]);
    logger::Log(input_args_string);

    logger::Log("Reading URL list");

    // Read links from input file
    std::ifstream url_file(argv[1]);
    std::vector<file::File> files;
    std::string url_file_line_buffer;
    while (std::getline(url_file, url_file_line_buffer))
    {
        if (url_file_line_buffer.length() != 0)
        {
            files.emplace_back(link_parser::ParseURL(url_file_line_buffer));
        }
    }

    // Create target directory if it doesn't exist
    if (!std::filesystem::is_directory(argv[2]))
    {
        std::filesystem::create_directories(argv[2]);
    }

    // Probe each link and excludes ones that return an error
    std::vector<long> excluded_indices;
    for (long i = 0;i < files.size();i++)
    {
        try
        {
            files[i].connect();
            files[i].get_info();
        }
        catch (std::exception& e)
        {
            excluded_indices.push_back(i);
            logger::LogError(files[i].get_url_target().host + files[i].get_url_target().target + " - " + e.what());
        }
    }
    while (!excluded_indices.empty())
    {
        files.erase(files.begin() + excluded_indices.back());
        excluded_indices.pop_back();
    }

    for (auto& file : files)
    {
        file.create_file(argv[2]);
    }

    std::vector<bool> file_crashed(files.size(), false);
    bool all_done = false;
    while (!all_done)
    {
        all_done = true;
        std::size_t concurrent_files = 0;
        for (int i = 0;i < files.size() && concurrent_files < max_concurrent_files;i++)
        {
            if (files[i].is_complete() || file_crashed[i])
            {
                continue;
            }

            try
            {
                files[i].read_some();
            }
            catch (std::exception& e)
            {
                logger::LogError(files[i].get_url_target().filename + " " + e.what());
                file_crashed[i] = true;
                continue;
            }
            concurrent_files++;

            if (!files[i].is_complete())
            {
                all_done = false;
            }
        }
    }

    logger::Log("Download complete.");

    return 0;
}