#include "commands/parser.hpp"

namespace commands {
  
  void Parser::next() {
    if(pos==string.end()) {
      tokenType = TokenType::End;
      return;
    }
    while(pos!=TokenType) {
      
    }
  }
  
  Parser::Parser(const std::string& str)
    : string(str)
    , pos(string.begin()){
    next();
  }
  
}