#include "requirements/commands/command_delete.hpp"

#include "requirements/nodecollection.hpp"

#include "requirements/status.hpp"

namespace requirements {
  namespace commands {
    
    Command_Delete::Command_Delete(::requirements::Id id)
      : selection(id) {}
    
    Command_Delete::Command_Delete(Parser &parser)
      : selection(parser) {}
    
    void Command_Delete::execute(Status &status) {
      auto storage = status.openStorage();
      auto node = selection.extractNode(*storage);
      storage->getNodeCollection().deleteNode(node);
    }
    
  }
}