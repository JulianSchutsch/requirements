#include "requirements/commands/select.hpp"

#include "requirements/select.hpp"
#include "requirements/status.hpp"

#include "requirements/commands/parser.hpp"

namespace requirements {
  namespace commands {
    
    void Select::execute(Status &status) {
      auto storage = status.openStorage();
      status.selections[0] = selection.extractIds(*storage);
    }
    
    Select::Select(std::vector<::requirements::Id> a_ids)
      : selection(a_ids) {}
    
    Select::Select(Parser &parser)
      : selection(parser) {}
    
  }
}