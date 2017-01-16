#pragma once

#include "requirements/icommand.hpp"
#include "requirements/id.hpp"

namespace requirements {
  namespace commands {
    
    class Parser;
    
    class Command_New final : public ICommand {
    private:
      bool idProvided = false;
      ::requirements::Id id;
    public:
      void execute(Status &status) override;
      
      Command_New(::requirements::Id a_id);
      
      Command_New(Parser &parser);
    };
    
  }
}