#pragma once

#include "icommand.hpp"
#include "selection.hpp"

namespace commands {
  
  class Status;
  class Parser;
  
  class Command_Delete : public ICommand {
    Selection selection;
  public:
    void execute(Status& status) override;
    Command_Delete(::requirements::Id id);
    Command_Delete(Parser& parser);
  };
  
}