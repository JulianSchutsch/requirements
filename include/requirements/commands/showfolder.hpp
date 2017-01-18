#pragma once

#include "requirements/icommand.hpp"

namespace requirements {
  class Status;
  namespace commands {
    class Parser;
    class ShowFolder : public ICommand {
    public:
      void execute(Status& status) override;
      ShowFolder(Parser& parser);
    };
  }
}