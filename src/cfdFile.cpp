#include "cfd/File.h"
#include "cfd/Logger.h"

#include <boost/beast/version.hpp>
#include <iostream>
#include <filesystem>

cfd::file::File::File(const cfd::URLParser::URLTarget& target) : target(target)
{
    ioc = std::make_unique<boost::beast::net::io_context>();

    ctx = std::make_unique<boost::asio::ssl::context>(boost::asio::ssl::context::tlsv12_client);
    (*ctx).set_verify_mode(boost::asio::ssl::context::verify_none);

    resolver = std::make_unique<boost::beast::net::ip::tcp::resolver>(*ioc);

    stream = std::make_unique<boost::asio::ssl::stream<boost::beast::tcp_stream>>(*ioc, *ctx);

    buffer = std::make_unique<boost::beast::flat_buffer>();
}

// cfd::file::File::File(const File& file) : target(file.target), ctx(boost::asio::ssl::context::tlsv12_client), resolver(ioc)
// {
//     ctx.set_verify_mode(boost::asio::ssl::context::verify_none);
// }

cfd::file::File::File(File&& file) : target(std::move(file.target)), filesize(std::move(file.filesize)), outfile(std::move(file.outfile)), ioc(std::move(file.ioc)), ctx(std::move(file.ctx)), resolver(std::move(file.resolver)), stream(std::move(file.stream)), buffer(std::move(file.buffer))
{
    
}

cfd::file::File::~File()
{
    if (outfile.is_open())
    {
        outfile.close();
    }

    if (stream != nullptr) {
        boost::beast::error_code ec;
        ec = (*stream).shutdown(ec);
    }
}

// cfd::file::File& cfd::file::File::operator=(const File& file)
// {
//     target = file.target;
//     ctx = boost::asio::ssl::context(boost::asio::ssl::context::tlsv12_client);
//     resolver = boost::beast::net::ip::tcp::resolver(ioc);

//     return *this;
// }

cfd::file::File& cfd::file::File::operator=(File&& file)
{
    target = std::move(file.target);
    filesize = std::move(file.filesize);
    outfile = std::move(file.outfile);

    ioc = std::move(file.ioc);
    ctx = std::move(file.ctx);
    resolver = std::move(file.resolver);
    stream = std::move(file.stream);
    buffer = std::move(file.buffer);

    return *this;
}

cfd::URLParser::URLTarget cfd::file::File::get_url_target() const
{
    return target;
}

void cfd::file::File::connect()
{
    boost::asio::ip::basic_resolver_results<boost::asio::ip::tcp> resolve_results = (*resolver).resolve(target.host, target.port);

    boost::beast::get_lowest_layer(*stream).connect(resolve_results);

    if (target.port == "443") 
    {
        (*stream).handshake(boost::asio::ssl::stream_base::client);
    }
}

void cfd::file::File::get_info()
{
    boost::beast::http::request<boost::beast::http::empty_body> request;
    request.method(boost::beast::http::verb::head); // Ask for header only
    request.target(target.target);
    request.version(11); // HTTP version 1.1
    request.set(boost::beast::http::field::host, target.host);
    request.set(boost::beast::http::field::user_agent, BOOST_BEAST_VERSION_STRING);

    if (target.port == "443") {
        boost::beast::http::write(*stream, request);
    }
    else {
        boost::beast::http::write(boost::beast::get_lowest_layer(*stream), request);
    }
    

    boost::beast::http::response_parser<boost::beast::http::empty_body> response_parser;
    response_parser.skip(true); // Skip body

    boost::beast::flat_buffer response_buffer;
    if (target.port == "443") {
        boost::beast::http::read_header(*stream, response_buffer, response_parser);
    }
    else {
        boost::beast::http::read_header(boost::beast::get_lowest_layer(*stream), response_buffer, response_parser);
    }

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

void cfd::file::File::create_file(const std::string& folder_path)
{
    bool filename_available = true;
    std::size_t additional_index = 0;
    do
    {
        filename_available = true;
        for (const auto& file : std::filesystem::directory_iterator(folder_path))
        {
            std::string final_filename = target.filename;
            if (additional_index)
            {
                final_filename += "(" + std::to_string(additional_index) + ")";
            }
            if (file.path().filename().string() == final_filename)
            {
                filename_available = false;
                additional_index++;
                break;
            }
        }
    } while (!filename_available);

    if (additional_index)
    {
        target.filename += "(" + std::to_string(additional_index) + ")";
    }

    std::filesystem::path final_path = std::filesystem::path(folder_path) / std::filesystem::path(target.filename);
    outfile.open(final_path);
}

void cfd::file::File::read_some()
{
    constexpr std::size_t buffer_size = 1024;

    if (download_complete) return;

    if (!get_request_initialized)
    {
        boost::beast::http::request<boost::beast::http::empty_body> request;
        request.method(boost::beast::http::verb::get); // Ask for header only
        request.target(target.target);
        request.version(11); // HTTP version 1.1
        request.set(boost::beast::http::field::host, target.host);
        request.set(boost::beast::http::field::user_agent, BOOST_BEAST_VERSION_STRING);

        if (target.port == "443") {
            boost::beast::http::write(*stream, request);
        }
        else {
            boost::beast::http::write(boost::beast::get_lowest_layer(*stream), request);
        }
        

        response_parser.body_limit(std::numeric_limits<std::size_t>::max());

        if (target.port == "443") {
            boost::beast::http::read_header(*stream, response_buffer, response_parser);
        }
        else {
            boost::beast::http::read_header(boost::beast::get_lowest_layer(*stream), response_buffer, response_parser);
        }
        

        get_request_initialized = true;
    }

    char buf[buffer_size];
    response_parser.get().body().data = buf;
    response_parser.get().body().size = sizeof(buf);

    std::size_t chunk_size;
    if (target.port == "443") {
        chunk_size = boost::beast::http::read_some(*stream, response_buffer, response_parser);
    }
    else {
        chunk_size = boost::beast::http::read_some(boost::beast::get_lowest_layer(*stream), response_buffer, response_parser);
    }
    
    read_bytes += chunk_size;

    outfile.write(buf, chunk_size);

    if (response_parser.is_done())
    {
        outfile.close();
        cfd::logger::Log(target.filename + " is done.");
        download_complete = true;
    }
}

float cfd::file::File::get_progress()
{
    return static_cast<float>(read_bytes) / static_cast<float>(filesize);
}

bool cfd::file::File::is_complete()
{
    return read_bytes >= filesize;
}
