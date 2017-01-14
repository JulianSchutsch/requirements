#pragma once

#include <vector>
#include <string>

#include "commands/icommand.hpp"
#include "requirements/id.hpp"

namespace commands {

  class Parser;

  class Command_Select final : public ICommand {
  private:
    enum class SelectionKind {
      ByIds,
      ByPatterns
    };
    SelectionKind selectionKind;
    std::vector<::requirements::Id> ids;
    std::vector<std::string> patterns;
  public:
    void execute(Status& status) override;
    Command_Select(std::vector<::requirements::Id> a_ids);
    Command_Select(Parser& parser);
  };

}