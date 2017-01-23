#include "requirements/commands/newblob.hpp"

#include "requirements/exception.hpp"
#include "requirements/status.hpp"
#include "requirements/blob.hpp"
#include "requirements/commands/parser.hpp"

namespace requirements {
  namespace commands {
    
    void NewBlob::execute(Status &status) {
      auto storage = status.openStorage();
      std::string id = importBlob(*storage, filename);
      if(id.empty()) {
        throw Exception(Exception::Kind::User, "Failed to import blob file %1%.", {filename});
      }
    }
    
    NewBlob::NewBlob(const std::string &a_filename)
      : filename(a_filename) {}
    NewBlob::NewBlob(Parser &parser) {
      if(!parser.nextSimple()) {
        throw Exception(Exception::Kind::User, "newblob requires filename as parameter.");
      }
      filename = parser.getTokenString();
      if(parser.nextSimple()) {
        throw Exception(Exception::Kind::User, "newblob can only accept a single filename parameter.");
      }
    }
  }
}