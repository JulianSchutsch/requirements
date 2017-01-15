#include <gtest/gtest.h>

#include "commands/parser.hpp"

static const char * content1 = "c1 cb c2";
static const char * content2 = "10 Hello Remaining text\n";

TEST(Commands, Parser) {
  {
    commands::Parser parser(content1);
    ASSERT_EQ(parser.next(), true);
    ASSERT_EQ(parser.getTokenType(), commands::Parser::TokenType::Symbol);
    ASSERT_EQ(parser.getTokenString(), "c1");
    ASSERT_EQ(parser.next(), true);
    ASSERT_EQ(parser.getTokenType(), commands::Parser::TokenType::Symbol);
    ASSERT_EQ(parser.getTokenString(), "cb");
    ASSERT_EQ(parser.next(), true);
    ASSERT_EQ(parser.getTokenType(), commands::Parser::TokenType::Symbol);
    ASSERT_EQ(parser.getTokenString(), "c2");
    ASSERT_EQ(parser.next(), false);
  }
  {
    commands::Parser parser(content2);
    ASSERT_EQ(parser.next(), true);
    ASSERT_EQ(parser.getTokenType(), commands::Parser::TokenType::Integer);
    ASSERT_EQ(parser.getTokenString(), "10");
    ASSERT_EQ(parser.getRemaining(), "Hello Remaining text\n");
  }
}

TEST(Commands, ParseEmtpy) {
  commands::Parser parser("");
  ASSERT_EQ(parser.getRemaining(), "");
}

TEST(Commands, ParseSimple) {
  commands::Parser parser("%1ksodkosdk \"30493=04?59");
  ASSERT_EQ(parser.nextSimple(), true);
  ASSERT_EQ(parser.getTokenString(), "%1ksodkosdk");
  ASSERT_EQ(parser.nextSimple(), true);
  ASSERT_EQ(parser.getTokenString(), "\"30493=04?59");
  ASSERT_EQ(parser.nextSimple(), false);
}