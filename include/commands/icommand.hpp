#pragma once

namespace commands {
  
  class ICommand {
  public:
    virtual void execute()=0;
    virtual ~ICommand() {}
  };
  
}