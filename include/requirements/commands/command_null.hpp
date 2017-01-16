#pragma once

#include "requirements/icommand.hpp"

namespace requirements {
  class Status;
  namespace commands {
    
    
    class Command_Null : public ICommand {
    public:
      virtual void execute(Status &status) override { (void) status; }
    };
    
  }
}