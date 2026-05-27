#include "cfd/Logger.h"

#include <iostream>
#include <sstream>
#include <chrono>

static std::string GetTimestampPrefix()
{
    // TODO Convert chrono timestamp to local timezone
    std::stringstream timestamp_prefix_stream;
    timestamp_prefix_stream << "[" << std::chrono::system_clock::now() << "] ";
    return timestamp_prefix_stream.str();
}

void cfd::logger::Log(const std::string& message)
{
    auto timestamp_prefix = GetTimestampPrefix();

    std::cout << timestamp_prefix << message << std::endl;
}

void cfd::logger::LogError(const std::string& error_message)
{
    auto timestamp_prefix = GetTimestampPrefix();

    std::cerr << timestamp_prefix << error_message << std::endl;
}