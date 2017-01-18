#pragma once

#include "requirements/icommand.hpp"

namespace requirements {
  class Status;
  namespace commands {
    class Parser;
    class Print : public ICommand {
    public:
      void execute(Status& status) override;
      Print();
      Print(Parser& parser);
    };
  }
}