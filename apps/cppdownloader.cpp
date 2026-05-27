#include "cfd/Logger.h"

namespace logger = cfd::logger;

int main(const int argc, const char * argv[])
{
    if (argc != 4)
    {
        logger::LogError("Wrong number of arguments");

        return 1;
    }

    logger::Log("C++ File Downloader starting");

    return 0;
}