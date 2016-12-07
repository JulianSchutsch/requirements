#pragma once

#include <string>
#include <memory>

#include "requirements/id.hpp"
#include "requirements/istorage.hpp"

namespace boost {
  namespace interprocess {
    class file_lock;
  }
}

namespace requirements {
  
  class NodeCollection;
  
  namespace storage {
    
    class Text final : public IStorage {
    private:
      NodeCollection& collection;
      std::string folder;
      std::unique_ptr<boost::interprocess::file_lock> fileLock;
    public:
      std::string createBlob(const std::string& suffix) override;
      std::string openBlob(const std::string& id) override;
      void closeBlob(const std::string& id) override;
      std::vector<std::string> getBlobs() override;
      Text(NodeCollection& collection, const std::string& folder);
      ~Text();
    };
    
  }
}