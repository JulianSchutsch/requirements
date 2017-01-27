#pragma once

#include <string>

#include "requirements/icommand.hpp"

namespace requirements {
  class Status;
  namespace commands {
    class Parser;
    class Help : public ICommand {
    private:
      std::string specificCommand;
    public:
      void execute(Status& status);
      Help(Parser& parser);
    };
  }
}