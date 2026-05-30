#include <iostream>

#include "cfd/URLParser.h"

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
    } state = host_part;
    std::size_t last_slash_index = 0;
    for (auto it = (url.begin() + protocol_declaration_skip); it != url.end(); it++)
    {
        if (state == host_part)
        {
            result.host.push_back(*it);
            if (*(it + 1) == '/')
            {
                state = target_part;
            }
        }
        else if (state == target_part)
        {
            if (*it == '/')
            {
                last_slash_index = it - url.begin();
            }
            result.target.push_back(*it);
        }
    }

    for (auto it = (url.begin() + last_slash_index + 1); it != url.end(); it++)
    {
        result.filename.push_back(*it);
    }

    return result;
}
