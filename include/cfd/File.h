#ifndef CPP_FILE_DOWNLOADER_FILE_H
#define CPP_FILE_DOWNLOADER_FILE_H

#include "cfd/URLParser.h"

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl.hpp>

namespace cfd
{
    namespace file
    {
        class File
        {
            URLParser::URLTarget target;
            std::size_t filesize = 0;

            boost::beast::net::io_context ioc;
            boost::asio::ssl::context ctx;
            boost::beast::net::ip::tcp::resolver resolver;
            boost::asio::ssl::stream<boost::beast::tcp_stream> stream = boost::asio::ssl::stream<boost::beast::tcp_stream>(ioc, ctx);
            boost::beast::flat_buffer buffer;

        public:
            File() = delete;
            File(const URLParser::URLTarget& target);
            File(const File& file);
            File(File&& file) noexcept;
            ~File();

            File& operator=(const File& file);
            File& operator=(File&& file) noexcept;

            URLParser::URLTarget get_url_target() const;

            void connect();
            void get_info();
        };
    }
}

#endif //CPP_FILE_DOWNLOADER_FILE_H
