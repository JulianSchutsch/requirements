#pragma once

#include <string>

#include "requirements/icommand.hpp"

namespace requirements {
  class Status;
  namespace commands {
    class Parser;
    class BlobAliases : public ICommand {
    private:
      std::string id;
      std::string aliases;
    public:
      void execute(Status& status) override;
      BlobAliases(const std::string& a_id, const std::string& a_aliases);
      BlobAliases(Parser& parser);
    };
  }
}