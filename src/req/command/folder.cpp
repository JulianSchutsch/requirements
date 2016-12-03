#include "req/command/folder.hpp"

#include "req/status.hpp"
#include "req/exception.hpp"

namespace req {
  namespace command {
    void processCommand_folder(Status& status, const std::vector<std::string>& parameters) {
      
      if(parameters.size()!=2) {
        throw Exception("Requires parameter: Folder");
      }
      status.folder = parameters[1];
    }
  }
}