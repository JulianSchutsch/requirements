#include "requirements/storage/text_load.hpp"

#include <fstream>
#include <vector>

#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>
#include <include/requirements/exception.hpp>

#include "util/path.hpp"
#include "util/stringfile.hpp"

#include "requirements/node.hpp"
#include "requirements/nodecollection.hpp"

#include "requirements/storage/text_common.hpp"

namespace requirements {
  namespace storage {

    static void readRequirements(NodeCollection& collection, const std::string& folder) {
      auto requirementsFolder = folder + text_requirementsFolder;
      for (auto it = boost::filesystem::directory_iterator(requirementsFolder);
           it != boost::filesystem::directory_iterator(); ++it) {
        boost::filesystem::path path(*it);
        Id id;
        if (!string_to_id(path.stem().string(), id)) {
          throw Exception(Exception::Kind::User, "Invalid file %1% not conforming to uuid standard found in %2%.",
                          {path.native(), requirementsFolder});
        }
        {
          try {
            auto content = util::readFileToString(path.native());
            collection.createNode(id, std::move(content));
          }
          catch(boost::filesystem::filesystem_error & e) {
            throw Exception(Exception::Kind::Other, "Cannot read file %1% in folder %2%.", {path.native(), requirementsFolder});
          }
        }
      }
    }
    
    static void readRelationships(NodeCollection& collection, const std::string& folder) {
      std::string filename = folder+text_relationshipsFile;
      if(!boost::filesystem::exists(filename)) {
        return;
      }
      if(boost::filesystem::is_directory(filename)) {
        throw Exception(Exception::Kind::Internal, "Relationsships file %1% must not be a folder.", {filename});
      }
      std::fstream file(filename, std::fstream::in);
      std::string line;
      while(std::getline(file, line)) {
        std::vector<std::string> parts;
        boost::algorithm::split(parts, line, boost::is_any_of(" "));
        auto it = parts.begin();
        if(it==parts.end()) {
          throw Exception(Exception::Kind::Internal, "Relationships file %1% has invalid line (empty line).", {filename});
        }
        NodePtr parent;
        if(*it=="root") {
          parent = collection.getRootNode();
        }
        else {
          Id id;
          if (!string_to_id(*it, id)) {
            throw Exception(Exception::Kind::Internal, "Relationshops file %1% has non uuid part.", {filename});
          }
          parent = collection.getNodeById(id);
        }
        if(!parent) {
          throw Exception(Exception::Kind::Internal, "Relationsships file %1% has invalid id reference.", {filename});
        }
        for(++it;it!=parts.end();++it) {
          Id id;
          if(!string_to_id(*it, id)) {
            throw Exception(Exception::Kind::Internal, "Relationships file %1% has non uuid part.", {filename});
          }
          auto node = collection.getNodeById(id);
          if(!node) {
            throw Exception(Exception::Kind::Internal, "Relationsships file %1% has invalid id reference.", {filename});
          }
          node->setLastOf(parent);
        }
      }
      
    }

    void text_load(NodeCollection& collection, const std::string& a_folder) {
      const auto& folder = util::ensureTrailingSlash(a_folder);
      if(folder.empty()) {
        throw Exception(Exception::Kind::Internal, "Folder name is empty", {});
      }
      text_ensureFolder(folder);
      readRequirements(collection, folder);
      readRelationships(collection, folder);
    }

  }
}