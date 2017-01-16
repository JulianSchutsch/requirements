#pragma once

#include "requirements/istorage.hpp"
#include "requirements/node.hpp"

#include "icommand.hpp"
#include "selection.hpp"
#include "status.hpp"

namespace commands {
   
  class Status;
  class Parser;
  
  template<void operation(::requirements::NodePtr)>
  class Move : public ICommand {
  private:
    Selection selection;
  public:
    void execute(Status& status) override {
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
    void execute(Status& status) override {
      auto storage = status.openStorage();
      auto element = selectionElement.extractNode(*storage, status);
      auto anchor = selectionAnchor.extractNode(*storage);
      operation(element, anchor);
    }
    
    AnchoredMove(::requirements::Id element, ::requirements::Id anchor)
      : selectionElement(element)
      , selectionAnchor(anchor) {}
    
    AnchoredMove(Parser& parser)
      : selectionAnchor(parser) {}
  };
  
}