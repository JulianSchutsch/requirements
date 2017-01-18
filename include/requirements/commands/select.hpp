#pragma once

#include <vector>
#include <string>

#include "requirements/icommand.hpp"
#include "requirements/commands/internal/selection.hpp"

namespace requirements {
  namespace commands {
    
    class Parser;
    
    class Select final : public ICommand {
    private:
      internal::Selection selection;
    public:
      void execute(Status &status) override;
      
      Select(std::vector<::requirements::Id> a_ids);
      
      Select(Parser &parser);
    };
    
  }
}