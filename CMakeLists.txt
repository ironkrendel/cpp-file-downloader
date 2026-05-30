cmake_minimum_required(VERSION 3.5.1...4.0.0)

project(cpp-file-downloader LANGUAGES CXX)

find_package(OpenSSL REQUIRED)
find_package(Boost REQUIRED)

add_library(cfdLogger STATIC include/cfd/Logger.h src/cfdLogger.cpp)
target_include_directories(cfdLogger PUBLIC include)
set_property(TARGET cfdLogger PROPERTY CXX_STANDARD 11)

add_executable(cppdownloader apps/cppdownloader.cpp)
target_link_libraries(cppdownloader PRIVATE cfdLogger)
set_property(TARGET cppdownloader PROPERTY CXX_STANDARD 11)