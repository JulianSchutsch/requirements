#include "req/command/delete.hpp"

#include <iostream>

#include "requirements/select.hpp"
#include "requirements/nodecollection.hpp"
#include "requirements/storage/text.hpp"

#include "req/status.hpp"

namespace req {
  namespace command {
    void processCommand_delete(Status& status, const std::vector<std::string>& parameters) {
      requirements::storage::Text storage(status.folder, true);
      auto& collection = storage.getNodeCollection();
      std::vector<requirements::NodePtr> selections;
      if(parameters.size()!=0) {
        selections = requirements::select(collection, parameters);
      } else {
        if(!requirements::selectFromIds(collection, status.selections[0], selections)) {
          std::cout<<"Invalid selection"<<std::endl;
          return;
        }
      }
      if(selections.size()!=1) {
        std::cout<<"delete command requires exactly one requirement selected"<<std::endl;
        return;
      }
      requirements::NodePtr node = selections[0];
      std::cout<<"Delete node with id="<<::requirements::id_to_string(node->getId())<<std::endl;
      collection.deleteNode(node);
      status.selections.clear();
    }
  }
}