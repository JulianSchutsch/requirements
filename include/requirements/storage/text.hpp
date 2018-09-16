#pragma once

#include <string>
#include <memory>

#include "requirements/id.hpp"
#include "requirements/istorage.hpp"
#include "requirements/nodecollection.hpp"

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
      NodeCollection collection{};
      std::string folder;
      std::unique_ptr<boost::interprocess::file_lock> fileLock;
      std::map<std::string, std::string> blobAliases{};

      bool autosave;

      void loadBlobAliases();
      void saveBlobAliases();
      void loadTrashNode();
      void saveTrashNode();

    public:
      std::string createBlob(const std::string& suffix) override;
      std::string getBlobFilename(const std::string& id) override;
      std::string getBlobSuffix(const std::string& id) override;
      std::string getLatexFolder() override;
      std::vector<std::string> getBlobs() override;
      std::map<std::string, std::string> getBlobAliases() override;
      void save(const std::string& save) override;
      void save() override;
      void setBlobAliases(const std::string& blob, const std::string& alias) override;
      NodeCollection& getNodeCollection() override { return collection; }
      Text(const std::string& folder, bool a_autosave);
      ~Text();
      Text(const Text&) = delete;
      Text& operator = (const Text&) = delete;
    };
    
  }
}