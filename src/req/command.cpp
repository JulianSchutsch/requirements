#include "req/command.hpp"

#include <map>
#include <iostream>

#include "req/exception.hpp"
#include "req/status.hpp"

#include "req/command/folder.hpp"
#include "req/command/showfolder.hpp"

namespace req {
  
  std::map<std::string, void(*)(Status&, const std::vector<std::string>&)> commands {
    {"folder", &command::processCommand_folder},
    {"showfolder", &command::processCommand_showFolder}
  };
  
  void processCommand(Status& status, const std::vector<std::string>& a_commands) {
    if(a_commands.empty()) {
      return;
    }
    auto it = commands.find(a_commands[0]);
    if(it==commands.end()) {
      throw Exception("Invalid command");
    }
    it->second(status, a_commands);
  }

}