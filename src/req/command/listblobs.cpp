#include "req/command/listblobs.hpp"

#include <iostream>

#include "requirements/nodecollection.hpp"
#include "requirements/storage/text.hpp"

#include "req/status.hpp"

namespace req {
  namespace command {
    void processCommand_listBlobs(Status& status, const std::vector<std::string>& parameters) {
      (void)parameters;
      requirements::storage::Text storage(status.folder, false);
      auto list = storage.getBlobs();
      for(auto& element: list) {
        std::cout<<element<<std::endl;
      }
    }
  }
}