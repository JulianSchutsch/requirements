#pragma once

namespace commands {
  
  class Status;
  
  class ICommand {
  public:
    virtual void execute(Status& status)=0;
    virtual ~ICommand() {}
  };
  
}