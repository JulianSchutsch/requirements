#pragma once

#include <string>
#include <memory>

namespace boost {
  namespace interprocess {
    class file_lock;
  }
}

namespace requirements {
  
  class NodeCollection;
  
  namespace storage {
    
    class Text final {
    private:
      NodeCollection& collection;
      std::string folder;
      std::unique_ptr<boost::interprocess::file_lock> fileLock;
    public:
      Text(NodeCollection& collection, const std::string& folder);
      ~Text();
    };
    
  }
}