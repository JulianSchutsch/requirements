#pragma once

#include <vector>
#include <string>

#include "commands/icommand.hpp"
#include "commands/selection.hpp"

namespace commands {

  class Parser;

  class Command_Select final : public ICommand {
  private:
    Selection selection;
  public:
    void execute(Status& status) override;
    Command_Select(std::vector<::requirements::Id> a_ids);
    Command_Select(Parser& parser);
  };

}