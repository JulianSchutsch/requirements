#include "requirements/commands/command.hpp"

#include <map>

#include "requirements/exception.hpp"

#include "requirements/icommand.hpp"
#include "requirements/commands/parser.hpp"
#include "requirements/commands/new.hpp"
#include "requirements/commands/select.hpp"
#include "requirements/commands/up.hpp"
#include "requirements/commands/down.hpp"
#include "requirements/commands/firstof.hpp"
#include "requirements/commands/lastof.hpp"
#include "requirements/commands/delete.hpp"
#include "requirements/commands/folder.hpp"
#include "requirements/commands/edit.hpp"
#include "requirements/commands/tree.hpp"
#include "requirements/commands/touch.hpp"
#include "requirements/commands/showfolder.hpp"
#include "requirements/commands/print.hpp"
#include "requirements/commands/newblob.hpp"
#include "requirements/commands/listblobs.hpp"
#include "requirements/commands/nextto.hpp"
#include "requirements/commands/previousto.hpp"

namespace requirements {
  namespace commands {
    
    using FactoryFunction = std::function<ICommand *(Parser &)>;
    
    static std::map<std::string, FactoryFunction> commands = {
      {"new",     [](Parser &parser) { return new New(parser); }},
      {"select",  [](Parser &parser) { return new Select(parser); }},
      {"up",      [](Parser &parser) { return new Up(parser); }},
      {"down",    [](Parser &parser) { return new Down(parser); }},
      {"firstof", [](Parser &parser) { return new FirstOf(parser); }},
      {"lastof",  [](Parser &parser) { return new LastOf(parser); }},
      {"delete",  [](Parser &parser) { return new Delete(parser); }},
      {"folder",  [](Parser &parser) { return new Folder(parser); }},
      {"edit", [](Parser& parser) { return new Edit(parser); }},
      {"tree", [](Parser& parser) { return new Tree(parser); }},
      {"touch", [](Parser& parser) { return new Touch(parser); }},
      {"showfolder", [](Parser& parser) { return new ShowFolder(parser); }},
      {"print", [](Parser& parser) { return new Print(parser); }},
      {"newblob", [](Parser& parser) { return new NewBlob(parser); }},
      {"listblobs", [](Parser& parser) { return new ListBlobs(parser); }},
      {"nextto", [](Parser& parser) { return new NextTo(parser); }},
      {"previousto", [](Parser& parser) { return new PreviousTo(parser); }}
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