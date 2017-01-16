#pragma once

#include <string>

#include "requirements/icommand.hpp"

namespace requirements {

  class Status;

  namespace commands {
    
    class Parser;
    
    class Command_Folder : public ICommand {
    private:
      std::string folder;
    public:
      void execute(Status &status);
      
      Command_Folder(const std::string &a_folder);
      
      Command_Folder(Parser &parser);
    };
    
  }
}