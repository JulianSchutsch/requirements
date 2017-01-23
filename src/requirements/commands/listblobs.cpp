#include "requirements/commands/listblobs.hpp"

#include "requirements/exception.hpp"
#include "requirements/status.hpp"
#include "requirements/istorage.hpp"
#include "requirements/commands/parser.hpp"

namespace requirements {
  namespace commands {
    ListBlobs::ListBlobs(Parser &parser) {
      if(parser.getRemaining()!="") {
        throw Exception(Exception::Kind::User, "Listblobs has no parameters.");
      }
    }
    void ListBlobs::execute(Status& status) {
      auto storage = status.openStorage();
      auto list = storage->getBlobAliases();
      for(auto& pair: list) {
        status.messageFunction(Status::MessageKind::Content, pair.first+"->"+pair.second);
      }
    }
  }
}