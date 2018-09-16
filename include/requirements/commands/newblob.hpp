#pragma once

#include "requirements/icommand.hpp"

namespace requirements {
  class Status;
  namespace commands {
    class Parser;
    class NewBlob : public ICommand {
    private:
      std::string filename{};
    public:
      void execute(Status& status) override;
      NewBlob(const std::string& a_filename);
      NewBlob(Parser& parser);
    };
  }
}