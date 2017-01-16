#include "commands/command.hpp"

#include <map>

#include "annotations/exception.hpp"

#include "commands/parser.hpp"
#include "commands/icommand.hpp"
#include "commands/command_new.hpp"
#include "commands/command_select.hpp"
#include "commands/command_up.hpp"
#include "commands/command_down.hpp"
#include "commands/command_firstof.hpp"
#include "commands/command_lastof.hpp"
#include "commands/command_delete.hpp"

namespace commands {
  
  using FactoryFunction = std::function<ICommand*(Parser&)>;
  
  static std::map<std::string, FactoryFunction> commands = {
    {"new", [](Parser& parser){return new Command_New(parser);}},
    {"select", [](Parser& parser){return new Command_Select(parser);}},
    {"up", [](Parser& parser){return new Command_Up(parser);}},
    {"down", [](Parser& parser){return new Command_Down(parser);}},
    {"firstof", [](Parser& parser){return new Command_FirstOf(parser);}},
    {"lastof", [](Parser& parser){return new Command_LastOf(parser);}},
    {"delete", [](Parser& parser){return new Command_Delete(parser);}}
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
    return std::unique_ptr<ICommand>(it->second(parser));
  }
}