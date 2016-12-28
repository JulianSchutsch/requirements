#include "req/command/newblob.hpp"

#include <iostream>

#include <boost/filesystem.hpp>

#include "requirements/nodecollection.hpp"
#include "requirements/node.hpp"
#include "requirements/storage/text.hpp"
#include "requirements/blob.hpp"

#include "req/status.hpp"

namespace req {
  namespace command {
    void processCommand_newBlob(Status& status, const std::vector<std::string>& parameters) {
      requirements::storage::Text storage(status.folder, true);
      auto& collection = storage.getNodeCollection();
      if(parameters.size()!=1) {
        std::cout<<"Require source file"<<std::endl;
        return;
      }
      auto sourceFile = parameters[0];
      if(!boost::filesystem::exists(sourceFile) || boost::filesystem::is_directory(sourceFile)) {
        std::cout<<"Invalid source file"<<std::endl;
        return;
      }
      std::string id = requirements::importBlob(storage, sourceFile);
      if(id.empty()) {
        std::cout<<"Failed to import blob"<<std::endl;
        return;
      }
      std::cout<<"Blob added:"<<id<<std::endl;
    }
  }
}