#include "req/command/setparent.hpp"

#include <iostream>

#include "req/status.hpp"

#include "requirements/storage/text.hpp"
#include "requirements/nodecollection.hpp"
#include "requirements/node.hpp"
#include "requirements/select.hpp"

namespace req {
  namespace command {
    void processCommand_setParent(Status& status, const std::vector<std::string>& parameters) {
      requirements::NodeCollection collection;
      requirements::storage::Text storage(collection, status.folder);

      auto parents = requirements::select(collection, parameters);
      if(parents.size()!=1) {
        std::cout<<"Selection for parent must specify exactly one element"<<std::endl;
        return;
      }
      auto parent = parents[0];
      
      std::vector<requirements::NodePtr> nodes;
      if(!requirements::selectFromIds(collection, status.selections[0], nodes)) {
        std::cout<<"Invalid selection list"<<std::endl;
        return;
      }
      for(auto node:nodes) {
        std::cout<<"Set Parent for "<<requirements::id_to_string(node->getId())<<" : "<<requirements::id_to_string(parent->getId())<<std::endl;
        node->setParent(parent);
      }
    }
  }
}