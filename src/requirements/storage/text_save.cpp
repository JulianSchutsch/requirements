#include "requirements/storage/text_save.hpp"

#include <fstream>
#include <vector>

#include <boost/algorithm/string/join.hpp>
#include <boost/filesystem.hpp>

#include "requirements/node.hpp"
#include "requirements/nodecollection.hpp"
#include "requirements/id.hpp"

#include "util/path.hpp"

#include "requirements/storage/text_common.hpp"
#include "requirements/storage/exception.hpp"

namespace requirements {
  namespace storage {
    
    static void saveNode(Node& node, const std::string& folder) {
      {
        auto& content = node.getContent();
        std::fstream file(folder + text_requirementsFolder + id_to_string(node.getId()), std::fstream::out);
        file<<content;
      }
    }
    
    static void saveNodes(NodeCollection& collection, const std::string& folder) {
      for(auto& node: collection) {
        saveNode(*node, folder);
      }
    }

    static void saveRelationshipsOfNode(std::ostream& stream, Node& node, bool root) {
      auto& children = node.getChildren();
      if(children.empty()) {
        return;
      }
      std::vector<std::string> ids;
      ids.reserve(children.size()+1);
      if(!root) {
        ids.emplace_back(id_to_string(node.getId()));
      } else {
        ids.emplace_back("root");
      }
      for(auto& entry: children) {
        ids.emplace_back(id_to_string(entry->getId()));
      }
      stream<<boost::algorithm::join(ids, " ")<<"\n";
    }
    
    static void saveRelationships(NodeCollection& collection, const std::string& folder) {
      std::fstream file(folder+text_relationshipsFile, std::fstream::out);
      saveRelationshipsOfNode(file, *collection.getRootNode(), true);
      for(auto& node: collection) {
        saveRelationshipsOfNode(file, *node, false);
      }
    }

    static void deleteObsoleteNodes(NodeCollection& collection, const std::string& folder) {
      for(auto it=boost::filesystem::directory_iterator(folder+text_requirementsFolder);it!=boost::filesystem::directory_iterator();++it) {
        boost::filesystem::path path(*it);
        Id id;
        if(!string_to_id(path.stem().string(), id)) {
          throw Exception(Exception::Reason::InvalidId);
        }
        NodePtr dummy;
        if(!collection.findById(id, dummy)) {
          boost::filesystem::remove(path);
        }
      }
    }

    void text_save(NodeCollection& collection, const std::string& a_folder) {
      const auto& folder = util::ensureTrailingSlash(a_folder);
      if(folder.empty()) {
        throw Exception(Exception::Reason::FolderNameEmpty);
      }
      text_ensureFolder(folder);
      saveNodes(collection, folder);
      saveRelationships(collection, folder);
      deleteObsoleteNodes(collection, folder);
    }
  }
}