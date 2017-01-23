#include "requirements/commands/new.hpp"

#include "requirements/exception.hpp"

#include "requirements/istorage.hpp"
#include "requirements/nodecollection.hpp"

#include "requirements/status.hpp"
#include "requirements/commands/parser.hpp"

namespace requirements {
  namespace commands {
    
    void New::execute(Status &status) {
      auto storage = status.openStorage();
      auto &collection = storage->getNodeCollection();
      std::string nullStr;
      if (idProvided) {
        collection.createNode(id, std::move(nullStr));
        status.selections[0] = {id};
        return;
      }
      auto newNode = collection.createNode(std::move(nullStr));
      status.selections[0] = {newNode->getId()};
    }
    
    New::New(Parser &parser) {
      if (parser.getRemaining() != "") {
        throw Exception(Exception::Kind::User, "New does not have parameters.");
      }
    }
    
    New::New(::requirements::Id a_id)
      : idProvided(true), id(a_id) {}
  }
}