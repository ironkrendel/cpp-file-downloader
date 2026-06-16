#include <iostream>

#include "cfd/URLParser.h"

std::pair<std::string, std::string> cfd::URLParser::SplitFilename(const std::string& filename)
{
    std::pair<std::string, std::string> result;

    std::size_t last_period_index = filename.rfind('.');
    if (last_period_index != std::string::npos)
    {
        result.first = filename.substr(0, last_period_index);
        result.second = filename.substr(last_period_index);
    }
    else
    {
        result.first = filename;
        result.second = "";
    }

    return result;
}

cfd::URLParser::URLTarget cfd::URLParser::ParseURL(const std::string& url)
{
    cfd::URLParser::URLTarget result;

    long protocol_declaration_skip;
    if (url.find("https") == 0)
    {
        result.port = "443";

        // Deduct null terminator from string literal's size
        protocol_declaration_skip = sizeof "https://" - 1;
    }
    else
    {
        result.port = "80";

        // Deduct null terminator from string literal's size
        protocol_declaration_skip = sizeof "http://" - 1;
    }

    enum
    {
        host_part,
        target_part
    } url_state = host_part;
    std::size_t last_slash_index = 0;
    for (auto it = (url.begin() + protocol_declaration_skip); it != url.end(); it++)
    {
        if (url_state == host_part)
        {
            result.host.push_back(*it);
            if (*(it + 1) == '/')
            {
                url_state = target_part;
            }
        }
        else if (url_state == target_part)
        {
            if (*it == '/')
            {
                last_slash_index = it - url.begin();
            }
            result.target.push_back(*it);
        }
    }

    auto split_filename = SplitFilename(url.substr(last_slash_index + 1));
    result.filename = split_filename.first;
    result.extension = split_filename.second;

    return result;
}
