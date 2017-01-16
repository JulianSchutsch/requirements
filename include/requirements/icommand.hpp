#pragma once

#include <functional>

namespace requirements {
  
  class Status;
  
  class ICommand {
  public:
    enum MessageKind {
      Error,
      Content
    };
    using MessageFunction = std::function<void(MessageKind, const std::string&)>;
    virtual void execute(Status& status, const MessageFunction& function)=0;
    virtual ~ICommand() {}
  };
  
}