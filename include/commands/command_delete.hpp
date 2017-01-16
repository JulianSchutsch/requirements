#pragma once

#include "commands/icommand.hpp"
#include "commands/selection.hpp"

namespace commands {
  
  class Status;
  class Parser;
  
  class Command_Delete : public ICommand {
    Selection selection;
  public:
    void execute(Status& status);
    Command_Delete(::requirements::Id id);
    Command_Delete(Parser& parser);
  };
  
}