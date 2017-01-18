#pragma once

#include "requirements/icommand.hpp"

namespace requirements {
  
  class Status;
  
  namespace commands {
    
    class Parser;
    
    class Touch : public ICommand {
    public:
      void execute(Status& status) override;
      Touch(Parser& parser);
    };
    
  }
  
}