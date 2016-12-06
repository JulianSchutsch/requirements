#include "requirements/storage/text.hpp"

#include <fstream>

#include <boost/interprocess/sync/file_lock.hpp>

#include "util/path.hpp"

#include "requirements/storage/text_common.hpp"
#include "requirements/storage/text_load.hpp"
#include "requirements/storage/text_save.hpp"

namespace requirements {
  namespace storage {
    
    Id Text::createBlob() {
      return generateRandomId();
    }
    
    std::string Text::openBlob(Id id) {
      return folder+text_blobFolder+id_to_string(id);
    }
    
    void Text::closeBlob(Id id) {
      (void)id;
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