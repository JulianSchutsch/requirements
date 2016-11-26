#include "requirements/storage/text_load.hpp"

#include <iostream>
#include <fstream>

#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>

#include "util/path.hpp"

#include "requirements/node.hpp"
#include "requirements/nodecollection.hpp"
#include "requirements/icontent.hpp"
#include "requirements/contentfactory.hpp"

#include "requirements/storage/exception.hpp"
#include "requirements/storage/text_common.hpp"

namespace requirements {
  namespace storage {

    static void readRequirements(NodeCollection& collection, const std::string& requirementsFolder) {
      try {
        for(auto it=boost::filesystem::directory_iterator(requirementsFolder);it!=boost::filesystem::directory_iterator();++it) {
          boost::filesystem::path path(*it);
          Id id;
          if(!string_to_id(path.stem().string(), id)) {
            throw Exception(Exception::Reason::InvalidId);
          }
          const std::string& extensionString = path.extension().string();
          const std::string suffix(extensionString, 1, extensionString.size()-1);
          auto content = contentFactory(suffix);
          if(!content) {
            throw Exception(Exception::Reason::InvalidSuffix);
          }
          collection.createNode(id, std::move(content));
        }
      }
      catch(boost::filesystem::filesystem_error& e) {
        throw Exception(Exception::Reason::FailedToReadFolder);
      }
    }
    
    static void readRelationships(const std::string& folder) {
      std::string filename = folder+"relationships";
      if(!boost::filesystem::exists(filename)) {
        return;
      }
      if(boost::filesystem::is_directory(filename)) {
        throw Exception(Exception::Reason::InvalidRelationshipsFile);
      }
    }

    void text_load(NodeCollection& collection, const std::string& a_folder) {
      const auto& folder = util::ensureTrailingSlash(a_folder);
      const auto& requirementsFolder = folder+"requirements/";
      if(folder.empty()) {
        throw Exception(Exception::Reason::FolderNameEmpty);
      }
      text_ensureFolders(folder, requirementsFolder);
      readRequirements(collection, requirementsFolder);  
      readRelationships(folder);
      
    }

  }
}