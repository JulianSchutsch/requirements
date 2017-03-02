#include "requirements/commands/trash.hpp"

#include "requirements/status.hpp"
#include "requirements/nodecollection.hpp"

namespace requirements {
  namespace commands {
    void Trash::execute(Status& status) {
      auto storage = status.openStorage();
      auto node = selection.extractNode(*storage);
      auto& collection = storage->getNodeCollection();
      if(hasTrashId) {
        collection.ensureTrashNode(trashId);
      }
      collection.nodeToTrash(node);
    }

    Trash::Trash(::requirements::Id id, ::requirements::Id a_trashId)
      : selection(id) {
      hasTrashId = true;
      trashId = a_trashId;
    }
    Trash::Trash(Parser& parser)
      : selection(parser) {}
  }
}