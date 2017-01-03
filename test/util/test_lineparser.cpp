#include <gtest/gtest.h>

#include <regex>

#include "util/lineparser.hpp"

TEST(LineParser, Empty) {
  std::string content = "";
  util::LineParser lineParser(content);
  std::string dummy;
  ASSERT_EQ(lineParser.consume(dummy), false);
  std::smatch dummyMatches;
  ASSERT_EQ(lineParser.consume(std::regex("a"), dummyMatches), false);
}

TEST(LineParser, TwoLines) {
  std::string content = "section: SomeData\nSomeText\n";
  util::LineParser lineParser(content);
  std::smatch matches;
  ASSERT_EQ(lineParser.consume(std::regex(R"(\w+\:\s*(.*))"), matches), true);
  ASSERT_EQ(matches.size(), 2);
  ASSERT_EQ(matches[0], "section: SomeData");
  ASSERT_EQ(matches[1], "SomeData");
  std::string line;
  ASSERT_EQ(lineParser.consume(line), true);
  ASSERT_EQ(line, "SomeText");
  ASSERT_EQ(lineParser.consume(line), false);
}