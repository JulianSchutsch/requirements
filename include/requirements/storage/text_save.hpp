#pragma once

#include <string>

namespace requirements {
  
  class NodeCollection;
  
  namespace storage {
    
    void text_save(NodeCollection& collection, const std::string& folder);
    
  }
}