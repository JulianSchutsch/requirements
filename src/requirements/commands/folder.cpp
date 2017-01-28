#include "requirements/commands/folder.hpp"

#include <boost/filesystem.hpp>

#include "requirements/exception.hpp"
#include "requirements/commands/parser.hpp"
#include "requirements/status.hpp"

namespace requirements {
  namespace commands {
    
    void Folder::execute(Status &status) {
      status.folder = boost::filesystem::absolute(folder).native();
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
        throw requirements::Exception(Exception::Kind::User, "Unexpected second parameter.");
      }
    }
    
  }
}