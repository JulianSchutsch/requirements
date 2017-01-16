#include "commands/parser.hpp"

namespace commands {
  
  static bool isCharacter(char ch) {
    return (ch>='A' && ch<='Z') || (ch>='a' && ch<='z');
  }
  
  static bool isNumber(char ch) {
    return (ch>='0') && (ch<='9');
  }
  
  static bool isSpace(char ch) {
    return ch==' ';
  }

  bool Parser::nextSimple() {
    consumeSpace();
    if(pos==string.end()) {
      return false;
    }
    tokenStart = pos;
    while(pos!=string.end() && !isSpace(*pos)) {
      ++pos;
    }
    tokenType = TokenType::String;
    return true;
  }
  
  void Parser::consumeSpace() {
    while(pos!=string.end() && isSpace(*pos)) { ++pos; }
  }
  
  bool Parser::nextInteger() {
    tokenType = TokenType::Integer;
    while(pos!=string.end()) {
      auto ch = *pos;
      if(!isNumber(ch)) {
        if(isSpace(ch)) {
          return true;
        }
        tokenType = TokenType::Unknown;
        return false;
      }
      ++pos;
    }
    return true;
  }
  
  bool Parser::nextSymbol() {
    tokenType = TokenType::Symbol;
    while(pos!=string.end()) {
      auto ch = *pos;
      if(!isCharacter(ch) && !isNumber(ch)) {
        if(isSpace(ch)) {
          return true;
        }
        tokenType = TokenType::Unknown;
        return false;
      }
      ++pos;
    }
    return true;
  }
  
  bool Parser::next() {
    consumeSpace();
    tokenType = TokenType::End;
    if(pos==string.end()) {
      return false;
    }
    tokenStart = pos;
    auto ch = *pos;
    ++pos;
    if(isCharacter(ch)) {
      return nextSymbol();
    }
    if(isNumber(ch)) {
      return nextInteger();
    }
    tokenType = TokenType::Unknown;
    return false;
  }
  
  Parser::Parser(const std::string& str)
    : string(str)
    , pos(string.begin()){
  }
  
}