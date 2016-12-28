#include <iostream>
#include "req/command/select.hpp"

#include "req/status.hpp"

#include "requirements/nodecollection.hpp"
#include "requirements/storage/text.hpp"
#include "requirements/select.hpp"
#include "requirements/id.hpp"

namespace req {
  namespace command {
    void processCommand_select(Status& status, const std::vector<std::string>& parameters) {
      requirements::storage::Text storage(status.folder, false);
      auto& collection = storage.getNodeCollection();
      auto selection = requirements::select(collection, parameters);
      status.selections[0].clear();
      status.selections[0].reserve(selection.size());
      std::cout<<"Selection:"<<std::endl;
      for(auto& element: selection) {
        status.selections[0].emplace_back(element->getId());
        std::cout<<"Selected: "<<requirements::id_to_string(element->getId())<<std::endl;
      }
    }
  }
}
