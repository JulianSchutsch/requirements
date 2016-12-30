#include "req/command/delete.hpp"

#include <iostream>

#include "requirements/nodecollection.hpp"
#include "requirements/storage/text.hpp"

#include "req/status.hpp"
#include "req/select.hpp"
#include "req/exception.hpp"

namespace req {
  namespace command {
    void processCommand_delete(Status& status, const std::vector<std::string>& parameters) {
      requirements::storage::Text storage(status.folder, true);
      auto& collection = storage.getNodeCollection();
      auto selections =  selectNodes(status, storage, 0, parameters);
      if(selections.size()!=1) {
        throw Exception("Delete requires exactly one node to be selected");
      }
      requirements::NodePtr node = selections[0];
      std::cout<<"Delete node with id="<<::requirements::id_to_string(node->getId())<<std::endl;
      collection.deleteNode(node);
      status.selections.clear();
    }
  }
}