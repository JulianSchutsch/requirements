#include "commands/command.hpp"

#include "annotations/exception.hpp"

#include "commands/parser.hpp"

namespace commands {
  
  void assembleFromString(const std::string& str) {
    Parser parser(str);
    if(!parser.next() || parser.getTokenType()!=Parser::TokenType::Symbol) {
      throw annotations::Exception("Invalid or no command");
    }
    auto command = parser.getTokenString();
    
  }
}