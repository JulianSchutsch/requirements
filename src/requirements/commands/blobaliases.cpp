#include "requirements/commands/blobaliases.hpp"

#include "requirements/status.hpp"
#include "requirements/commands/parser.hpp"
#include "requirements/blob.hpp"
#include "requirements/exception.hpp"

namespace requirements {
  namespace commands {
    void BlobAliases::execute(Status &status) {
      auto storage = status.openStorage();
      auto selected = ::requirements::selectBlobs(*storage,{id});
      if(selected.size()!=1) {
        throw Exception("BlobAliases requires exactly one blob to be selected");
      }
      auto node = selected[0];
      storage->setBlobAliases(node, node+" "+aliases);
    }
    
    BlobAliases::BlobAliases(const std::string &a_id, const std::string &a_aliases)
      : id(a_id)
      , aliases(a_aliases) {}
    
    BlobAliases::BlobAliases(Parser& parser) {
      if(!parser.nextSimple()) {
        throw Exception("Expect blob id");
      }
      id = parser.getTokenString();
      aliases = parser.getRemaining();
    }
  }
}