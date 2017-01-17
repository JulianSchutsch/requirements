#include "requirements/commands/edit.hpp"

#include "requirements/status.hpp"

namespace requirements {
  namespace commands {
    
    Edit::Edit(Parser &parser)
      : selection(parser) {}
    
    void Edit::execute(Status &status) {
      auto storage = status.openStorage();
      auto node = selection.extractNode(*storage, status);
      status.editFunction(node);
    }
    
  }
}