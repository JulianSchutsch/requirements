#include "req/command/newblob.hpp"

#include <iostream>

#include <boost/filesystem.hpp>

#include "requirements/nodecollection.hpp"
#include "requirements/storage/text.hpp"
#include "requirements/blob.hpp"

#include "req/status.hpp"
#include "req/exception.hpp"

namespace req {
  namespace command {
    void processCommand_newBlob(Status& status, const std::vector<std::string>& parameters) {
      requirements::storage::Text storage(status.folder, true);
      auto& collection = storage.getNodeCollection();
      if(parameters.size()!=1) {
        throw Exception("Source file for blob required");
      }
      auto sourceFile = parameters[0];
      if(!boost::filesystem::exists(sourceFile) || boost::filesystem::is_directory(sourceFile)) {
        throw Exception("Source file does not exist or is not valid");
      }
      std::string id = requirements::importBlob(storage, sourceFile);
      if(id.empty()) {
        throw Exception("Failed to import blob");
      }
      std::cout<<"Blob added:"<<id<<std::endl;
    }
  }
}