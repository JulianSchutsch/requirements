#include "req/command/tree.hpp"

#include <iostream>

#include "requirements/storage/text.hpp"
#include "requirements/nodecollection.hpp"
#include "requirements/node.hpp"
#include "requirements/select.hpp"

#include "req/status.hpp"
#include "req/console/printtree.hpp"

namespace req {
  namespace command {
    void processCommand_tree(Status& status, const std::vector<std::string>& parameters) {
      requirements::storage::Text storage(status.folder, false);
      auto& collection = storage.getNodeCollection();
      
      auto selected = requirements::select(collection, parameters, collection.getRootNode());

      bool first = true;
      for(auto& element: selected) {
        if(!requirements::hasSelectedParent(element, selected)) {
          if(!first) {
            std::cout<<"-----------------------"<<std::endl;
          }
          req::console::printTree(std::cout, element, selected);
          first = false;
        }
      }
    }
  }
}