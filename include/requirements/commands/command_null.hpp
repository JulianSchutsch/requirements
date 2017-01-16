#pragma once

#include "icommand.hpp"

namespace commands {
  
  class Status;
  
  class Command_Null : public ICommand {
  public:
    virtual void execute(Status& status) override {(void)status; }
  };
  
}