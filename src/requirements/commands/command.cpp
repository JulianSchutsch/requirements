#include "requirements/commands/command.hpp"

#include <map>

#include "requirements/exception.hpp"

#include "requirements/icommand.hpp"
#include "requirements/commands/parser.hpp"
#include "requirements/commands/command_new.hpp"
#include "requirements/commands/command_select.hpp"
#include "requirements/commands/command_up.hpp"
#include "requirements/commands/command_down.hpp"
#include "requirements/commands/command_firstof.hpp"
#include "requirements/commands/command_lastof.hpp"
#include "requirements/commands/command_delete.hpp"
#include "requirements/commands/command_folder.hpp"

namespace requirements {
  namespace commands {
    
    using FactoryFunction = std::function<ICommand *(Parser &)>;
    
    static std::map<std::string, FactoryFunction> commands = {
      {"new",     [](Parser &parser) { return new Command_New(parser); }},
      {"select",  [](Parser &parser) { return new Command_Select(parser); }},
      {"up",      [](Parser &parser) { return new Command_Up(parser); }},
      {"down",    [](Parser &parser) { return new Command_Down(parser); }},
      {"firstof", [](Parser &parser) { return new Command_FirstOf(parser); }},
      {"lastof",  [](Parser &parser) { return new Command_LastOf(parser); }},
      {"delete",  [](Parser &parser) { return new Command_Delete(parser); }},
      {"folder",  [](Parser &parser) { return new Command_Folder(parser); }}
    };
    
    std::unique_ptr<ICommand> assembleFromString(const std::string &str) {
      Parser parser(str);
      if (!parser.next() || parser.getTokenType() != Parser::TokenType::Symbol) {
        throw Exception("Invalid or no command");
      }
      auto cmdStr = parser.getTokenString();
      auto it = commands.find(cmdStr);
      if (it == commands.end()) {
        throw Exception("Command not found");
      }
      return std::unique_ptr<ICommand>(it->second(parser));
    }
  }
}