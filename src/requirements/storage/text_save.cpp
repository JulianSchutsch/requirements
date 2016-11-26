#include "requirements/storage/text_save.hpp"

#include <fstream>
#include <vector>
#include <iostream>

#include <boost/algorithm/string/join.hpp>

#include "requirements/icontentvisitor.hpp"
#include "requirements/node.hpp"
#include "requirements/nodecollection.hpp"
#include "requirements/icontent.hpp"
#include "requirements/id.hpp"

#include "util/path.hpp"

#include "requirements/storage/text_common.hpp"
#include "requirements/storage/exception.hpp"

namespace requirements {
  namespace storage {
    namespace {
      class ExtractContentSuffix : public IContentVisitor {
      private:
        std::string& suffix;
      public:
        void handleContent(Content_Text& text) override {
          (void)text;
          suffix = ".txt";
        }
        ExtractContentSuffix(std::string& a_suffix)
          : suffix(a_suffix) {}
      };
    }
    
    static void saveNode(Node& node, const std::string& requirementsFolder) {
      auto& content = node.getContent();
      if(content.isModified()) {
        std::string suffix;
        content.visit(ExtractContentSuffix(suffix));
        std::fstream file(requirementsFolder+id_to_string(node.getId())+suffix, std::fstream::out);
        content.serialize(file);
      }
    }
    
    static void saveNodes(NodeCollection& collection, const std::string& requirementsFolder) {
      for(auto& node: collection) {
        saveNode(node, requirementsFolder);
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
      std::fstream file(folder+"relationships", std::fstream::out);
      for(auto& node: collection) {
        saveRelationshipsOfNode(file, node);
      }
    }

    void text_save(NodeCollection& collection, const std::string& a_folder) {
      const auto& folder = util::ensureTrailingSlash(a_folder);
      const auto& requirementsFolder = folder+"requirements/";
      if(folder.empty()) {
        throw Exception(Exception::Reason::FolderNameEmpty);
      }
      text_ensureFolders(folder, requirementsFolder);
      saveNodes(collection, requirementsFolder);
      saveRelationships(collection, folder);
    }
  }
}