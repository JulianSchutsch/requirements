#include "commands/command.hpp"

#include <map>

#include "annotations/exception.hpp"

#include "commands/parser.hpp"
#include "commands/icommand.hpp"
#include "commands/command_new.hpp"

#include <iostream>

namespace commands {

  using FactoryMethod = std::unique_ptr<ICommand>(*)(Parser&);

  static std::unique_ptr<ICommand> cmd_new(Parser& parser) {
    return std::unique_ptr<ICommand>(new Command_New(parser));
  }

  static std::map<std::string, FactoryMethod> commands = {
    {"new", &cmd_new}
  };
  
  std::unique_ptr<ICommand> assembleFromString(const std::string& str) {
    Parser parser(str);
    if(!parser.next() || parser.getTokenType()!=Parser::TokenType::Symbol) {
      throw annotations::Exception("Invalid or no command");
    }
    auto cmdStr = parser.getTokenString();
    auto it = commands.find(cmdStr);
    if(it==commands.end()) {
      throw annotations::Exception("Command not found");
    }
    return it->second(parser);
  }
}