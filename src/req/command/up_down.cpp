#include "req/command/up_down.hpp"

#include <iostream>

#include "requirements/nodecollection.hpp"
#include "requirements/storage/text.hpp"

#include "req/status.hpp"
#include "req/select.hpp"
#include "req/exception.hpp"

namespace req {
  namespace command {
    void processCommand_up(Status& status, const std::vector<std::string>& parameters) {
      requirements::storage::Text storage(status.folder, true);
      auto& collection = storage.getNodeCollection();
      auto selections = selectNodes(status, storage, 0, parameters);
      if(selections.size()!=1) {
        throw Exception("Up requires exactly one element selected");
      }
      requirements::NodePtr node = selections[0];
      node->up();
    }

    void processCommand_down(Status& status, const std::vector<std::string>& parameters) {
      requirements::storage::Text storage(status.folder, true);
      auto& collection = storage.getNodeCollection();
      auto selections = selectNodes(status, storage, 0, parameters);
      if(selections.size()!=1) {
        throw Exception("Down requires exactly one element selected");
      }
      requirements::NodePtr node = selections[0];
      node->down();
    }
  }
}