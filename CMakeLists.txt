cmake_minimum_required(VERSION 3.5.1...4.0.0)

project(cpp-file-downloader LANGUAGES CXX)
set(CPP_STANDARD 17)

find_package(OpenSSL REQUIRED)
find_package(Boost REQUIRED)

add_library(cfdLogger STATIC include/cfd/Logger.h src/cfdLogger.cpp)
target_include_directories(cfdLogger PUBLIC include)
set_property(TARGET cfdLogger PROPERTY CXX_STANDARD ${CPP_STANDARD})

add_library(cfdURLParser STATIC include/cfd/URLParser.h src/cfdURLParser.cpp)
target_include_directories(cfdURLParser PUBLIC include)
set_property(TARGET cfdURLParser PROPERTY CXX_STANDARD ${CPP_STANDARD})

add_library(cfdFile STATIC include/cfd/File.h src/cfdFile.cpp)
target_include_directories(cfdFile PUBLIC include)
target_link_libraries(cfdFile PRIVATE OpenSSL::SSL)
set_property(TARGET cfdFile PROPERTY CXX_STANDARD ${CPP_STANDARD})

add_executable(cppdownloader apps/cppdownloader.cpp)
target_link_libraries(cppdownloader PRIVATE cfdLogger)
target_link_libraries(cppdownloader PRIVATE cfdURLParser)
target_link_libraries(cppdownloader PRIVATE cfdFile)
set_property(TARGET cppdownloader PROPERTY CXX_STANDARD ${CPP_STANDARD})