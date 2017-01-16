#pragma once

#include "requirements/istorage.hpp"
#include "requirements/node.hpp"

#include "requirements/icommand.hpp"
#include "requirements/commands/internal/selection.hpp"
#include "requirements/status.hpp"

namespace requirements {
  namespace commands {

    class Parser;

    namespace internal {
      
      
      template<void operation(::requirements::NodePtr)>
      class Move : public ICommand {
      private:
        internal::Selection selection;
      public:
        void execute(Status &status, const MessageFunction&) override {
          auto storage = status.openStorage();
          auto node = selection.extractNode(*storage, status);
          operation(node);
        }
        
        Move(::requirements::Id id)
          : selection(id) {}
        
        Move(Parser &parser)
          : selection(parser) {}
      };
      
      template<void operation(::requirements::NodePtr, ::requirements::NodePtr)>
      class AnchoredMove : public ICommand {
      private:
        Selection selectionElement;
        Selection selectionAnchor;
      public:
        void execute(Status &status, const MessageFunction&) override {
          auto storage = status.openStorage();
          auto element = selectionElement.extractNode(*storage, status);
          auto anchor = selectionAnchor.extractNode(*storage);
          operation(element, anchor);
        }
        
        AnchoredMove(::requirements::Id element, ::requirements::Id anchor)
          : selectionElement(element), selectionAnchor(anchor) {}
        
        AnchoredMove(Parser &parser)
          : selectionAnchor(parser) {}
      };
      
    }
  }
}