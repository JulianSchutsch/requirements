#pragma once

#include "requirements/icommand.hpp"
#include "requirements/commands/internal/selection.hpp"

namespace requirements {

  class Status;

  namespace commands {
    
    class Parser;
    
    class Delete : public ICommand {
      internal::Selection selection;
    public:
      void execute(Status& status, const MessageFunction& msg) override;
      
      Delete(::requirements::Id id);
      
      Delete(Parser &parser);
    };
    
  }
}