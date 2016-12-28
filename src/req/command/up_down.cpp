#include "req/command/up_down.hpp"

#include <iostream>

#include "requirements/nodecollection.hpp"
#include "requirements/storage/text.hpp"
#include "requirements/select.hpp"

#include "req/status.hpp"

namespace req {
  namespace command {
    void processCommand_up(Status& status, const std::vector<std::string>& parameters) {
      requirements::NodeCollection collection;
      requirements::storage::Text storage(collection, status.folder);
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
        std::cout<<"up command requires exactly one requirement selected"<<std::endl;
        return;
      }
      requirements::NodePtr node = selections[0];
      node->up();
    }

    void processCommand_down(Status& status, const std::vector<std::string>& parameters) {
      requirements::NodeCollection collection;
      requirements::storage::Text storage(collection, status.folder);
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
        std::cout<<"down command requires exactly one requirement selected"<<std::endl;
        return;
      }
      requirements::NodePtr node = selections[0];
      node->down();
    }
  }
}