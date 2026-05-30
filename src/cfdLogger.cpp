#include "cfd/Logger.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <cmath>

static std::string GetTimestampPrefix()
{
    std::stringstream timestamp_prefix_stream;

    std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm tm_now = *std::localtime(&now);

    char time_string_buffer[64];
    std::strftime(time_string_buffer, sizeof time_string_buffer, "%Y-%m-%d %H:%M:%S", &tm_now);

    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() % 1000;

    timestamp_prefix_stream << "[" << time_string_buffer << "." << std::setfill('0') << std::setw(3) << millis << "] ";
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