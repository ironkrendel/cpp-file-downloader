#include "cfd/URLParser.h"

#include <gtest/gtest.h>

TEST(URLParserTests, FilenameSplitWithoutExtension)
{
    auto result = cfd::URLParser::SplitFilename("target");

    EXPECT_EQ(result.first, "target");
    EXPECT_EQ(result.second, "");
}

TEST(URLParserTests, FilenameSplitWithExtension)
{
    auto result = cfd::URLParser::SplitFilename("target.txt");

    EXPECT_EQ(result.first, "target");
    EXPECT_EQ(result.second, ".txt");
}

TEST(URLParserTests, WithoutExtension)
{
    cfd::URLParser::URLTarget result = cfd::URLParser::ParseURL("http://testsite.com/target");

    EXPECT_EQ(result.host, "testsite.com");
    EXPECT_EQ(result.port, "80");
    EXPECT_EQ(result.target, "/target");
    EXPECT_EQ(result.filename, "target");
    EXPECT_EQ(result.extension, "");

    result = cfd::URLParser::ParseURL("https://testsite.com/target");

    EXPECT_EQ(result.host, "testsite.com");
    EXPECT_EQ(result.port, "443");
    EXPECT_EQ(result.target, "/target");
    EXPECT_EQ(result.filename, "target");
    EXPECT_EQ(result.extension, "");
}

TEST(URLParserTests, WithExtension)
{
    cfd::URLParser::URLTarget result = cfd::URLParser::ParseURL("http://testsite.com/target.txt");

    EXPECT_EQ(result.host, "testsite.com");
    EXPECT_EQ(result.port, "80");
    EXPECT_EQ(result.target, "/target.txt");
    EXPECT_EQ(result.filename, "target");
    EXPECT_EQ(result.extension, ".txt");

    result = cfd::URLParser::ParseURL("https://testsite.com/target.txt");

    EXPECT_EQ(result.host, "testsite.com");
    EXPECT_EQ(result.port, "443");
    EXPECT_EQ(result.target, "/target.txt");
    EXPECT_EQ(result.filename, "target");
    EXPECT_EQ(result.extension, ".txt");
}

TEST(URLParserTests, NestedWithoutExtension)
{
    cfd::URLParser::URLTarget result = cfd::URLParser::ParseURL("http://testsite.com/folder/target");

    EXPECT_EQ(result.host, "testsite.com");
    EXPECT_EQ(result.port, "80");
    EXPECT_EQ(result.target, "/folder/target");
    EXPECT_EQ(result.filename, "target");
    EXPECT_EQ(result.extension, "");

    result = cfd::URLParser::ParseURL("https://testsite.com/folder/target");

    EXPECT_EQ(result.host, "testsite.com");
    EXPECT_EQ(result.port, "443");
    EXPECT_EQ(result.target, "/folder/target");
    EXPECT_EQ(result.filename, "target");
    EXPECT_EQ(result.extension, "");
}

TEST(URLParserTests, NestedWithExtension)
{
    cfd::URLParser::URLTarget result = cfd::URLParser::ParseURL("http://testsite.com/folder/target.txt");

    EXPECT_EQ(result.host, "testsite.com");
    EXPECT_EQ(result.port, "80");
    EXPECT_EQ(result.target, "/folder/target.txt");
    EXPECT_EQ(result.filename, "target");
    EXPECT_EQ(result.extension, ".txt");

    result = cfd::URLParser::ParseURL("https://testsite.com/folder/target.txt");

    EXPECT_EQ(result.host, "testsite.com");
    EXPECT_EQ(result.port, "443");
    EXPECT_EQ(result.target, "/folder/target.txt");
    EXPECT_EQ(result.filename, "target");
    EXPECT_EQ(result.extension, ".txt");
}

int main()
{
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}