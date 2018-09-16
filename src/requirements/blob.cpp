#include "requirements/blob.hpp"

#include <regex>

#include <boost/filesystem.hpp>

#include "requirements/istorage.hpp"

namespace requirements {

  std::vector<std::string> selectBlobs(IStorage& storage, const std::vector<std::string>& parameters) {
    std::vector<std::string> result;

    auto blobs = storage.getBlobAliases();

    std::vector<std::regex> regexes;
    regexes.reserve(parameters.size());
    for(auto& parameter: parameters) {
      regexes.emplace_back(parameter);
    }

    for(auto& pair: blobs) {
      bool matched = true;
      for(auto& e: regexes) {
        if(!std::regex_search(pair.second, e)) {
          matched = false;
          break;
        }
      }
      if(matched) {
        result.emplace_back(pair.first);
      }
    }
    return result;
  }

  std::string importBlob(IStorage& storage, const std::string& sourceFile) {
    boost::filesystem::path p(sourceFile);
    auto suffix = p.extension().native();
    if(!suffix.empty() && suffix[0]=='.') {
      suffix.erase(suffix.begin());
    }
    auto id = storage.createBlob(suffix);
    auto targetFile = storage.getBlobFilename(id);
    boost::filesystem::remove(targetFile);
    boost::filesystem::copy_file(sourceFile, targetFile);
    return id;
  }
  
}