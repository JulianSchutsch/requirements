#pragma once

#include "requirements/istorage.hpp"
#include "requirements/node.hpp"

#include "commands/icommand.hpp"
#include "commands/selection.hpp"
#include "commands/status.hpp"

namespace commands {
   
  class Status;
  
  template<void operation(::requirements::NodePtr)>
  class Move : public ICommand {
  private:
    Selection selection;
  public:
    void execute(Status& status) {
      auto storage = status.openStorage();
      auto node = selection.extractNode(*storage, status);
      operation(node);
    }
    Move(::requirements::Id id)
      : selection(id) {}
    Move(Parser& parser)
      : selection(parser) {}
  };
  
  template<void operation(::requirements::NodePtr, ::requirements::NodePtr)>
  class AnchoredMove : public ICommand {
  private:
    Selection selectionElement;
    Selection selectionAnchor;
  public:
    void execute(Status& status) {
      auto storage = status.openStorage();
      auto element = selectionElement.extractNode(*storage);
      auto anchor = selectionAnchor.extractNode(*storage, status);
      operation(element, anchor);
    }
  };
  
}