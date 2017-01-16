#pragma once

#include "requirements/icommand.hpp"

namespace requirements {
  class Status;
  namespace commands {
    
    class Null : public ICommand {
    public:
      virtual void execute(Status &status, const MessageFunction&) override { (void) status; }
    };
    
  }
}