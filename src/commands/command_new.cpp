#include "commands/command_new.hpp"

#include "annotations/exception.hpp"

#include "requirements/istorage.hpp"
#include "requirements/nodecollection.hpp"

#include "commands/status.hpp"
#include "commands/parser.hpp"

namespace commands {
  
  void Command_New::execute(Status& status) {
    auto storage = status.openStorage();
    auto& collection = storage->getNodeCollection();
    std::string nullStr;
    if(idProvided) {
      collection.createNode(id, std::move(nullStr));
      status.selections[0] = {id};
      return;
    }
    auto newNode = collection.createNode(std::move(nullStr));
    status.selections[0] = {newNode->getId()};
  }
  
  Command_New::Command_New(Parser& parser) {
    if(parser.getRemaining()!="") {
      throw annotations::Exception("No parameters for new command expected");
    }
  }
  
  Command_New::Command_New(::requirements::Id a_id)
    : idProvided(true)
    , id(a_id) {}
}