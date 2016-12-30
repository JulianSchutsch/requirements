#pragma once

#include <string>
#include <vector>
#include <map>

#include "requirements/id.hpp"

namespace requirements {

  class NodeCollection;
  
  class IStorage {
  public:
    virtual std::string createBlob(const std::string& suffix) = 0;
    virtual std::string getBlobFilename(const std::string& id) = 0;
    virtual std::string getBlobSuffix(const std::string& id) = 0;
    virtual std::vector<std::string> getBlobs() = 0;
    virtual std::map<std::string, std::string> getBlobAliases() = 0;
    virtual std::string getLatexFolder() = 0;
    virtual NodeCollection& getNodeCollection() = 0;
    virtual void save(const std::string& folder) = 0;
    virtual void save() = 0;
    virtual void setBlobAliases(const std::string& blob, const std::string& aliases) = 0;
  };
  
}