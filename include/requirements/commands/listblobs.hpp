#pragma once

#include "requirements/icommand.hpp"

namespace requirements {
  class Status;
  namespace commands {
    class Parser;
    class ListBlobs : public ICommand {
    public:
      void execute(Status& status) override;
      ListBlobs(Parser& parser);
    };
  }
}