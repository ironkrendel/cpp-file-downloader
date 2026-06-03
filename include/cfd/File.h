#ifndef CPP_FILE_DOWNLOADER_FILE_H
#define CPP_FILE_DOWNLOADER_FILE_H

#include "cfd/URLParser.h"

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl.hpp>
#include <fstream>

namespace cfd
{
    namespace file
    {
        class File
        {
            URLParser::URLTarget target;
            std::size_t filesize = 0;
            std::size_t read_bytes = 0;
            bool download_complete = false;

            boost::beast::net::io_context ioc;
            boost::asio::ssl::context ctx;
            boost::beast::net::ip::tcp::resolver resolver;
            boost::asio::ssl::stream<boost::beast::tcp_stream> stream = boost::asio::ssl::stream<boost::beast::tcp_stream>(ioc, ctx);
            boost::beast::flat_buffer buffer;

            std::ofstream outfile;

            bool get_request_initialized = false;
            boost::beast::http::response_parser<boost::beast::http::buffer_body> response_parser;
            boost::beast::flat_buffer response_buffer;

        public:
            File() = delete;
            File(const URLParser::URLTarget& target);
            File(const File& file);
            File(File&& file);
            ~File();

            File& operator=(const File& file);
            File& operator=(File&& file);

            URLParser::URLTarget get_url_target() const;

            void connect();
            void get_info();
            bool sanitize_name();
            void create_file(const std::string& folder_path);
            void read_some();
            float get_progress();
            bool is_complete();
        };
    }
}

#endif //CPP_FILE_DOWNLOADER_FILE_H
