#include "requirements/commands/delete.hpp"

#include "requirements/nodecollection.hpp"

#include "requirements/status.hpp"

namespace requirements {
  namespace commands {
    
    Delete::Delete(::requirements::Id id)
      : selection(id) {}
    
    Delete::Delete(Parser &parser)
      : selection(parser) {}
    
    void Delete::execute(Status &status) {
      auto storage = status.openStorage();
      auto node = selection.extractNode(*storage);
      storage->getNodeCollection().deleteNode(node);
    }
    
  }
}