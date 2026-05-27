cmake_minimum_required(VERSION 3.5.1...4.0.0)

project(cpp-file-downloader LANGUAGES CXX)

find_package(OpenSSL REQUIRED)
find_package(Boost REQUIRED)

add_executable(cppdownloader apps/cppdownloader.cpp)