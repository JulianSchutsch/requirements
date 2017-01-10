#include "requirements/storage/text_load.hpp"

#include <fstream>
#include <vector>

#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>

#include "util/path.hpp"
#include "util/stringfile.hpp"

#include "requirements/node.hpp"
#include "requirements/nodecollection.hpp"

#include "requirements/storage/exception.hpp"
#include "requirements/storage/text_common.hpp"

namespace requirements {
  namespace storage {

    static void readRequirements(NodeCollection& collection, const std::string& folder) {
      try {
        for(auto it=boost::filesystem::directory_iterator(folder+text_requirementsFolder);it!=boost::filesystem::directory_iterator();++it) {
          boost::filesystem::path path(*it);
          Id id;
          if(!string_to_id(path.stem().string(), id)) {
            throw Exception(Exception::Reason::InvalidId);
          }
          {
            auto content = util::readFileToString(path.native());
            auto node = collection.createNode(id, std::move(content));
          }
        }
      }
      catch(boost::filesystem::filesystem_error& e) {
        throw Exception(Exception::Reason::FailedToReadFolder);
      }
    }
    
    static void readRelationships(NodeCollection& collection, const std::string& folder) {
      std::string filename = folder+text_relationshipsFile;
      if(!boost::filesystem::exists(filename)) {
        return;
      }
      if(boost::filesystem::is_directory(filename)) {
        throw Exception(Exception::Reason::InvalidRelationshipsFile);
      }
      std::fstream file(filename, std::fstream::in);
      std::string line;
      while(std::getline(file, line)) {
        std::vector<std::string> parts;
        boost::algorithm::split(parts, line, boost::is_any_of(" "));
        auto it = parts.begin();
        if(it==parts.end()) {
          throw Exception(Exception::Reason::InvalidRelationshipsFile);
        }
        NodePtr parent;
        if(*it=="root") {
          parent = collection.getRootNode();
        }
        else {
          Id id;
          if (!string_to_id(*it, id)) {
            throw Exception(Exception::Reason::InvalidRelationshipsFile);
          }
          parent = collection.getNodeById(id);
        }
        if(!parent) {
          throw Exception(Exception::Reason::ReferenceToUnknownId);
        }
        for(++it;it!=parts.end();++it) {
          Id id;
          if(!string_to_id(*it, id)) {
            throw Exception(Exception::Reason::InvalidRelationshipsFile);
          }
          auto node = collection.getNodeById(id);
          if(!node) {
            throw Exception(Exception::Reason::ReferenceToUnknownId);
          }
          node->setParent(parent);
        }
      }
      
    }

    void text_load(NodeCollection& collection, const std::string& a_folder) {
      const auto& folder = util::ensureTrailingSlash(a_folder);
      if(folder.empty()) {
        throw Exception(Exception::Reason::FolderNameEmpty);
      }
      text_ensureFolder(folder);
      readRequirements(collection, folder);  
      readRelationships(collection, folder);
      
    }

  }
}