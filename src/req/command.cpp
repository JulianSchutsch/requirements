#include "req/command.hpp"

#include <map>

#include "req/exception.hpp"
#include "req/status.hpp"

#include "req/command/folder.hpp"
#include "req/command/showfolder.hpp"
#include "req/command/tree.hpp"
#include "req/command/new.hpp"
#include "req/command/select.hpp"
#include "req/command/setparent.hpp"
#include "req/command/edit.hpp"
#include "req/command/newblob.hpp"
#include "req/command/listblobs.hpp"

namespace req {
  
  std::map<std::string, void(*)(Status&, const std::vector<std::string>&)> commands {
    {"folder", &command::processCommand_folder},
    {"showfolder", &command::processCommand_showFolder},
    {"tree", &command::processCommand_tree},
    {"new", &command::processCommand_new},
    {"select", &command::processCommand_select},
    {"setparent", &command::processCommand_setParent},
    {"edit", &command::processCommand_edit},
    {"newblob", &command::processCommand_newBlob},
    {"listblobs", &command::processCommand_listBlobs},
  };
  
  void processCommand(Status& status, const std::vector<std::string>& a_commands) {
    if(a_commands.empty()) {
      return;
    }
    auto it = commands.find(a_commands[0]);
    if(it==commands.end()) {
      throw Exception("Invalid command");
    }
    auto itNext = std::begin(a_commands);
    ++itNext;
    std::vector<std::string> parameters(itNext, std::end(a_commands));
    it->second(status, parameters);
  }

}