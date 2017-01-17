#include "requirements/commands/newblob.hpp"

#include "requirements/exception.hpp"
#include "requirements/status.hpp"
#include "requirements/blob.hpp"
#include "requirements/commands/parser.hpp"

#include <iostream>

namespace requirements {
  namespace commands {
    
    void NewBlob::execute(Status &status) {
      auto storage = status.openStorage();
      std::string id = importBlob(*storage, filename);
      if(id.empty()) {
        throw Exception("Failed to import blob: "+filename);
      }
    }
    
    NewBlob::NewBlob(const std::string &a_filename)
      : filename(a_filename) {}
    NewBlob::NewBlob(Parser &parser) {
      if(!parser.nextSimple()) {
        throw Exception("Missing filename");
      }
      filename = parser.getTokenString();
      if(parser.nextSimple()) {
        throw Exception("Only one filename expected");
      }
    }
  }
}