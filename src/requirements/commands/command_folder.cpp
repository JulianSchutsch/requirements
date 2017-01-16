#include "requirements/commands/command_folder.hpp"

#include "requirements/exception.hpp"

#include "requirements/commands/parser.hpp"
#include "requirements/status.hpp"

namespace requirements {
  namespace commands {
    
    void Command_Folder::execute(Status &status) {
      status.folder = folder;
    }
    
    Command_Folder::Command_Folder(const std::string &a_folder)
      : folder(a_folder) {}
    
    Command_Folder::Command_Folder(Parser &parser) {
      if (!parser.nextSimple()) {
        folder = "";
        return;
      }
      folder = parser.getTokenString();
      if (parser.nextSimple()) {
        throw requirements::Exception("Unexpected second parameter");
      }
    }
    
  }
}