#include "req/command/showfolder.hpp"

#include <iostream>

#include "req/status.hpp"
#include "req/exception.hpp"

namespace req {
  namespace command {
    void processCommand_showFolder(Status& status, const std::vector<std::string>& parameters) {
      if(parameters.size()!=1) {
        throw Exception("Too many parameters");
      }
      std::cout<<"Current root folder: "<<status.folder<<std::endl;
    }
  }
}