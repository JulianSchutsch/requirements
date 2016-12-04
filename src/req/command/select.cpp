#include "req/command/select.hpp"

#include "req/status.hpp"

#include "requirements/nodecollection.hpp"
#include "requirements/storage/text.hpp"

namespace req {
  namespace command {
    void processCommand_select(Status& status, const std::vector<std::string>& parameters) {
      requirements::NodeCollection collection;
      requirements::storage::Text storage(collection, status.folder);
      // Call common selection library
    }
  }
}