#pragma once

#include <vector>
#include <string>

#include "requirements/icommand.hpp"
#include "requirements/commands/internal/selection.hpp"

namespace requirements {
  namespace commands {
    
    class Parser;
    
    class Command_Select final : public ICommand {
    private:
      internal::Selection selection;
    public:
      void execute(Status &status) override;
      
      Command_Select(std::vector<::requirements::Id> a_ids);
      
      Command_Select(Parser &parser);
    };
    
  }
}