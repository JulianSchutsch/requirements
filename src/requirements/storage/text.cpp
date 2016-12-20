#include "requirements/storage/text.hpp"

#include <fstream>
#include <iostream>

#include <boost/interprocess/sync/file_lock.hpp>
#include <boost/filesystem.hpp>

#include "util/path.hpp"

#include "requirements/storage/text_common.hpp"
#include "requirements/storage/text_load.hpp"
#include "requirements/storage/text_save.hpp"

namespace requirements {
  namespace storage {
    
    std::vector<std::string> Text::getBlobs() {
      std::vector<std::string> result;
      for(auto it=boost::filesystem::directory_iterator(folder+text_blobFolder);it!=boost::filesystem::directory_iterator();++it) {
        boost::filesystem::path path(*it);
        auto id = path.filename().native();
        result.emplace_back(std::move(id));
      }
      return std::move(result);
    }
    
    std::string Text::createBlob(const std::string& suffix) {
      return id_to_string(generateRandomId()) + "." + suffix;
    }
    
    std::string Text::getBlobFilename(const std::string& id) {
      return folder + text_blobFolder+id;
    }

    std::string Text::getLatexFolder() {
      return folder + text_latexFolder;
    }

    Text::Text(NodeCollection& a_collection, const std::string& a_folder)
      : collection(a_collection)
      , folder(util::ensureTrailingSlash(a_folder)) {
      text_ensureFolder(folder);
      std::string lockFilename = folder+"lock";
      { std::fstream s(lockFilename, std::fstream::out); }
      fileLock.reset(new boost::interprocess::file_lock(lockFilename.c_str()));
      text_load(collection, folder);
    }
    
    Text::~Text() {
      text_save(collection, folder);
    }
    
  }
}