#include "commands/command_folder.hpp"

#include "annotations/exception.hpp"

#include "commands/parser.hpp"
#include "commands/status.hpp"

namespace commands {
  
  void Command_Folder::execute(Status& status) {
    status.folder = folder;
  }
  
  Command_Folder::Command_Folder(const std::string& a_folder)
    : folder(a_folder) {}
  
  Command_Folder::Command_Folder(Parser& parser) {
    if(!parser.nextSimple()) {
      folder = "";
      return;
    }
    folder = parser.getTokenString();
    if(parser.nextSimple()) {
      throw annotations::Exception("Unexpected second parameter");
    }
  }
  
}