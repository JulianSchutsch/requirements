#include "req/command/new.hpp"

#include <iostream>

#include "requirements/nodecollection.hpp"
#include "requirements/storage/text.hpp"
#include "requirements/id.hpp"

#include "req/status.hpp"
#include "req/exception.hpp"

namespace req{
  namespace command{
    void processCommand_new(Status& status, const std::vector<std::string>& parameters) {
      if(parameters.size()!=0) {
        throw Exception("No parameters for new expected");
      }
      requirements::NodeCollection collection;
      requirements::storage::Text(collection, status.folder);
      auto node = collection.createNode("");
      std::cout<<"Node with id="<<requirements::id_to_string(node->getId())<<" created and selected"<<std::endl;
      status.selections[0] = {requirements::id_to_string(node->getId())};
      
    }
  }
}