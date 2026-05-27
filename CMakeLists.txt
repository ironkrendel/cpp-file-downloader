cmake_minimum_required(VERSION 3.5.1...4.0.0)

project(cpp-file-downloader LANGUAGES CXX)

find_package(OpenSSL REQUIRED)
find_package(Boost REQUIRED)

add_library(cfdLogger STATIC include/cfd/Logger.h src/cfdLogger.cpp)
target_include_directories(cfdLogger PUBLIC include)
target_compile_features(cfdLogger PUBLIC cxx_std_11)

add_executable(cppdownloader apps/cppdownloader.cpp)
target_link_libraries(cppdownloader PRIVATE cfdLogger)
target_compile_features(cppdownloader PRIVATE cxx_std_11)