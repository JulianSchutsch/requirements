#pragma once

#include <string>

#include "requirements/icommand.hpp"

namespace requirements {

  class Status;

  namespace commands {
    
    class Parser;
    
    class Folder : public ICommand {
    private:
      std::string folder;
    public:
      void execute(Status &status) override;
      
      Folder(const std::string &a_folder);
      
      Folder(Parser &parser);
    };
    
  }
}