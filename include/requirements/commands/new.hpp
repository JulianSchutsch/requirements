#pragma once

#include "requirements/icommand.hpp"
#include "requirements/id.hpp"

namespace requirements {
  namespace commands {
    
    class Parser;
    
    class New final : public ICommand {
    private:
      bool idProvided = false;
      ::requirements::Id id{};
    public:
      void execute(Status &status) override;
      
      New(::requirements::Id a_id);
      
      New(Parser &parser);
    };
    
  }
}