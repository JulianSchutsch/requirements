#include "req/command/edit.hpp"

#include <iostream>
#include <fstream>

#include <boost/filesystem.hpp>

#include "util/process.hpp"
#include "util/stringfile.hpp"

#include "req/status.hpp"

#include "requirements/nodecollection.hpp"
#include "requirements/node.hpp"
#include "requirements/storage/text.hpp"
#include "requirements/select.hpp"

namespace req {
  namespace command {
    void processCommand_edit(Status& status, const std::vector<std::string>& parameters) {
      requirements::NodeCollection collection;
      requirements::storage::Text storage(collection, status.folder);
      std::vector<requirements::NodePtr> selections;
      std::cout<<parameters.size()<<std::endl;
      if(parameters.size()!=0) {
        selections = requirements::select(collection, parameters);
      } else {
        std::cout<<"Select from ids"<<std::endl;
        if(!requirements::selectFromIds(collection, status.selections[0], selections)) {
          std::cout<<"Invalid selection"<<std::endl;
          return;
        }
      }
      if(selections.size()!=1) {
        std::cout<<"edit command requires exactly one requirement selected"<<std::endl;
        return;
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