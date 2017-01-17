#include "requirements/commands/showfolder.hpp"

#include "requirements/status.hpp"
#include "requirements/exception.hpp"
#include "requirements/commands/parser.hpp"

namespace requirements {
  namespace commands {
    void ShowFolder::execute(Status &status) {
      status.messageFunction(Status::MessageKind::Message, status.folder);
    }
    
    ShowFolder::ShowFolder(Parser &parser) {
      if(parser.getRemaining()!="") {
        throw Exception("showfolder does not accept any parameters");
      }
    }
  }
}