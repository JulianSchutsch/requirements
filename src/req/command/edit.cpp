#include "req/command/edit.hpp"

#include <iostream>
#include <fstream>

#include <boost/filesystem.hpp>

#include "util/process.hpp"
#include "util/stringfile.hpp"

#include "requirements/nodecollection.hpp"
#include "requirements/storage/text.hpp"

#include "req/status.hpp"
#include "req/select.hpp"
#include "req/exception.hpp"

namespace req {
  namespace command {
    void processCommand_edit(Status& status, const std::vector<std::string>& parameters) {
      requirements::storage::Text storage(status.folder, true);
      auto& collection = storage.getNodeCollection();
      auto selections = selectNodes(status, storage, 0, parameters);
      if(selections.size()!=1) {
        throw Exception("Edit requires exactly one node to be selected");
      }
      requirements::NodePtr node = selections[0];
      boost::filesystem::path tempFilename = boost::filesystem::unique_path();
      {
        std::fstream f(tempFilename.native(), std::fstream::out);
        f<<node->getContent();
      }
      if(util::runProcess("/bin/nano", {tempFilename.native()})) {
        node->updateContent(util::readFileToString(tempFilename.native()));
      } else {
        std::cout<<"Failed running editor"<<std::endl;
      }
      boost::filesystem::remove_all(tempFilename);
    }
  }
}