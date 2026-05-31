#include "cfd/File.h"

#include <boost/beast/version.hpp>
#include <iostream>

cfd::file::File::File(const cfd::URLParser::URLTarget& target) : target(target), ctx(boost::asio::ssl::context::tlsv12_client), resolver(ioc)
{
    ctx.set_verify_mode(boost::asio::ssl::context::verify_none);
}

cfd::file::File::File(const File& file) : target(file.target), ctx(boost::asio::ssl::context::tlsv12_client), resolver(ioc)
{
    ctx.set_verify_mode(boost::asio::ssl::context::verify_none);
}

cfd::file::File::File(File&& file) noexcept : target(std::move(file.target)), ctx(boost::asio::ssl::context::tlsv12_client), resolver(ioc)
{

}

cfd::file::File::~File()
{
    boost::beast::error_code ec;
    ec = stream.shutdown(ec);
}

cfd::file::File& cfd::file::File::operator=(const File& file)
{
    target = file.target;
    ctx = boost::asio::ssl::context(boost::asio::ssl::context::tlsv12_client);
    resolver = boost::beast::net::ip::tcp::resolver(ioc);

    return *this;
}

cfd::file::File& cfd::file::File::operator=(File&& file) noexcept
{
    target = std::move(file.target);
    ctx = boost::asio::ssl::context(boost::asio::ssl::context::tlsv12_client);
    resolver = boost::beast::net::ip::tcp::resolver(ioc);

    return *this;
}

cfd::URLParser::URLTarget cfd::file::File::get_url_target() const
{
    return target;
}

void cfd::file::File::connect()
{
    boost::asio::ip::basic_resolver_results<boost::asio::ip::tcp> resolve_results = resolver.resolve(target.host, target.port);

    boost::beast::get_lowest_layer(stream).connect(resolve_results);

    stream.handshake(boost::asio::ssl::stream_base::client);
}

void cfd::file::File::get_info()
{
    boost::beast::http::request<boost::beast::http::empty_body> request;
    request.method(boost::beast::http::verb::head); // Ask for header only
    request.target(target.target);
    request.version(11); // HTTP version 1.1
    request.set(boost::beast::http::field::host, target.host);
    request.set(boost::beast::http::field::user_agent, BOOST_BEAST_VERSION_STRING);

    boost::beast::http::write(stream, request);

    boost::beast::http::response_parser<boost::beast::http::empty_body> response_parser;
    response_parser.skip(true); // Skip body

    boost::beast::flat_buffer response_buffer;
    boost::beast::http::read_header(stream, response_buffer, response_parser);

    if (response_parser.get().result_int() != 200)
    {
        throw std::runtime_error(std::to_string(response_parser.get().result_int()));
    }

    for (const auto& field : response_parser.get())
    {
        // std::cout << field.name_string() << " " << field.value() << std::endl;
        if (field.name_string() == "Content-Length")
        {
            filesize = std::stoul(field.value());
        }
        else if (field.name_string() == "Content-Disposition" && field.value().find("attachment") != -1 && field.value().find("filename") != -1)
        {
            target.filename = "";
            for (auto it = (field.value().begin() + field.value().find("filename") + sizeof "filename='" - 1);it != field.value().end() && *it != '"'; it++)
            {
                target.filename.push_back(*it);
            }
        }
    }
}
