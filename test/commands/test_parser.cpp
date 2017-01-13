#include <gtest/gtest.h>

#include "commands/parser.hpp"

const char * content1 = "c1 cb c2";
const char * content2 = "10 Hello Remaining text\n";

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
  }
  {
    commands::Parser parser(content2);
    ASSERT_EQ(parser.next(), true);
    ASSERT_EQ(parser.getTokenType(), commands::Parser::TokenType::Integer);
    ASSERT_EQ(parser.getTokenString(), "10");
    ASSERT_EQ(parser.getRemaining(), "Hello Remaining text\n");
  }
}