#include "requirements/commands/command_select.hpp"

#include "requirements/select.hpp"
#include "requirements/status.hpp"

#include "requirements/commands/parser.hpp"

namespace requirements {
  namespace commands {
    
    void Command_Select::execute(Status &status) {
      auto storage = status.openStorage();
      status.selections[0] = selection.extractIds(*storage);
    }
    
    Command_Select::Command_Select(std::vector<::requirements::Id> a_ids)
      : selection(a_ids) {}
    
    Command_Select::Command_Select(Parser &parser)
      : selection(parser) {}
    
  }
}