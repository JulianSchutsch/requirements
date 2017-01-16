#pragma once

#include "requirements/icommand.hpp"
#include "requirements/commands/internal/selection.hpp"

namespace requirements {

  class Status;

  namespace commands {
    
    class Parser;
    
    class Command_Delete : public ICommand {
      internal::Selection selection;
    public:
      void execute(Status& status) override;
      
      Command_Delete(::requirements::Id id);
      
      Command_Delete(Parser &parser);
    };
    
  }
}