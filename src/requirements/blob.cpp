#include "requirements/blob.hpp"

#include <iostream>

#include <boost/filesystem.hpp>

#include "requirements/istorage.hpp"

namespace requirements {
  
  std::string importBlob(IStorage& storage, const std::string& sourceFile) {
    boost::filesystem::path p(sourceFile);
    auto suffix = p.extension().native();
    if(!suffix.empty() && suffix[0]=='.') {
      suffix.erase(suffix.begin());
    }
    auto id = storage.createBlob(suffix);
    auto targetFile = storage.openBlob(id);
    boost::filesystem::copy_file(sourceFile, targetFile);
    storage.closeBlob(id);
    return id;
  }
  
}