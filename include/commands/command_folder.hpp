#pragma once

#include <string>

#include "commands/icommand.hpp"

namespace commands {
  
  class Status;
  class Parser;
  
  class Command_Folder : public ICommand {
  private:
    std::string folder;
  public:
    void execute(Status& status);
    Command_Folder(const std::string& a_folder);
    Command_Folder(Parser& parser);
  };
  
}