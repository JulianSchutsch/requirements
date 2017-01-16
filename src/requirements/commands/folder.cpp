#include "requirements/commands/folder.hpp"

#include "requirements/exception.hpp"

#include "requirements/commands/parser.hpp"
#include "requirements/status.hpp"

namespace requirements {
  namespace commands {
    
    void Folder::execute(Status &status, const MessageFunction&) {
      status.folder = folder;
    }
    
    Folder::Folder(const std::string &a_folder)
      : folder(a_folder) {}
    
    Folder::Folder(Parser &parser) {
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