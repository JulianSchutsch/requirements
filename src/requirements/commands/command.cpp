#include "requirements/commands/command.hpp"

#include <map>
#include <algorithm>

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
#include "requirements/commands/showfolder.hpp"
#include "requirements/commands/print.hpp"
#include "requirements/commands/newblob.hpp"
#include "requirements/commands/listblobs.hpp"
#include "requirements/commands/nextto.hpp"
#include "requirements/commands/previousto.hpp"
#include "requirements/commands/blobaliases.hpp"
#include "requirements/commands/help.hpp"
#include "requirements/commands/levelup.hpp"
#include "requirements/commands/leveldown.hpp"

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
      {"showfolder", [](Parser& parser) { return new ShowFolder(parser); }},
      {"print", [](Parser& parser) { return new Print(parser); }},
      {"newblob", [](Parser& parser) { return new NewBlob(parser); }},
      {"listblobs", [](Parser& parser) { return new ListBlobs(parser); }},
      {"nextto", [](Parser& parser) { return new NextTo(parser); }},
      {"previousto", [](Parser& parser) { return new PreviousTo(parser); }},
      {"blobaliases", [](Parser& parser) { return new BlobAliases(parser); }},
      {"help", [](Parser& parser) { return new Help(parser); }},
      {"levelup", [](Parser& parser) { return new LevelUp(parser); }},
      {"leveldown", [](Parser& parser) { return new LevelDown(parser); }}
    };
    
    std::unique_ptr<ICommand> assembleFromString(const std::string &str) {
      Parser parser(str);
      if (!parser.next() || parser.getTokenType() != Parser::TokenType::Symbol) {
        throw Exception(Exception::Kind::User, "Invalid or no command.");
      }
      auto cmdStr = parser.getTokenString();
      std::transform(cmdStr.begin(), cmdStr.end(), cmdStr.begin(), ::tolower);
      auto it = commands.find(cmdStr);
      if (it == commands.end()) {
        throw Exception(Exception::Kind::User, "Command %1% not found.", {cmdStr});
      }
      return std::unique_ptr<ICommand>(it->second(parser));
    }
  }
}