#pragma once

#include <functional>

#include "requirements/node.hpp"

namespace requirements {
  
  class Status;
  
  class ICommand {
  public:
    virtual void execute(Status& status)=0;
    virtual ~ICommand() {}
  };
  
}