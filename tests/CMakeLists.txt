cmake_minimum_required(VERSION 3.10...4.0.0)

project(cpp-file-downloader-tests LANGUAGES CXX)
set(CPP_STANDARD 17)

include(GoogleTest)

find_package(GTest)

add_executable(URLParserTests cfdURLParserTests.cpp)
target_link_libraries(URLParserTests PRIVATE cfdURLParser)
target_link_libraries(URLParserTests PRIVATE GTest::gtest_main)
target_include_directories(URLParserTests PUBLIC ../include)
set_property(TARGET URLParserTests PROPERTY CXX_STANDARD ${CPP_STANDARD})

gtest_discover_tests(URLParserTests)