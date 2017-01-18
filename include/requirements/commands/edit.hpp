#pragma once

#include "requirements/icommand.hpp"
#include "requirements/commands/internal/selection.hpp"

namespace requirements {
  
  class Status;
  
  namespace commands {

    class Parser;

    class Edit : public ICommand {
    private:
      internal::Selection selection;
    public:
      void execute(Status& status) override;
      Edit(Parser& parser);
    };
  }
}