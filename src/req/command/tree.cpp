#include "req/command/tree.hpp"

#include "requirements/storage/text.hpp"
#include "requirements/nodecollection.hpp"

#include "req/status.hpp"

namespace req {
  namespace command {
    void processCommand_tree(Status& status, const std::vector<std::string>& parameters) {
      requirements::NodeCollection collection;
      requirements::storage::Text(collection, status.folder);
    }
  }
}