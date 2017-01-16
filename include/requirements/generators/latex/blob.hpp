#pragma once

#include <string>

namespace requirements {
  class IStorage;
  namespace generators {
    namespace latex {
      enum class BlobType {
        Image
      };
      
      struct BlobDescription {
        BlobType type;
        std::string filename;
      };
      
      bool translateBlob(::requirements::IStorage &storage, const std::string &description, BlobDescription &result);
    }
  }
}