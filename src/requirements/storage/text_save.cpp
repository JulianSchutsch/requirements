#include "requirements/storage/text_save.hpp"

#include <fstream>
#include <vector>
#include <iostream>

#include <boost/algorithm/string/join.hpp>

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
      {
        auto& annotations = node.getAnnotations();
        std::fstream file(folder + text_annotationsFolder + id_to_string(node.getId()), std::fstream::out);
        file<<annotations;
      }
    }
    
    static void saveNodes(NodeCollection& collection, const std::string& folder) {
      std::cout<<"Save nodes"<<std::endl;
      for(auto& node: collection) {
        std::cout<<id_to_string(node.getId())<<std::endl;
        saveNode(node, folder);
      }
    }

    static void saveRelationshipsOfNode(std::ostream& stream, Node& node) {
      auto& children = node.getChildren();
      if(children.empty()) {
        return;
      }
      std::vector<std::string> ids;
      ids.reserve(children.size()+1);
      ids.emplace_back(id_to_string(node.getId()));
      for(auto& entry: children) {
        ids.emplace_back(id_to_string(entry->getId()));
      }
      stream<<boost::algorithm::join(ids, " ")<<"\n";
    }
    
    static void saveRelationships(NodeCollection& collection, const std::string& folder) {
      std::fstream file(folder+text_relationshipsFile, std::fstream::out);
      for(auto& node: collection) {
        saveRelationshipsOfNode(file, node);
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
    }
  }
}