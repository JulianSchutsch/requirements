#include "requirements/commands/tree.hpp"

#include "requirements/status.hpp"
#include "requirements/select.hpp"
#include "requirements/console/printtree.hpp"

namespace requirements {
  namespace commands {
    void Tree::execute(Status &status) {
      auto storage = status.openStorage();
  
      auto selected = selection.extractNodes(*storage);
  
      bool first = true;
      for(auto& element: selected) {
        if(!requirements::hasSelectedParent(element, selected)) {
          if(!first) {
            status.messageFunction(Status::MessageKind::Content, "----------------------------------");
          }
          console::printTree([&status](const std::string& msg){status.messageFunction(Status::MessageKind::Content, msg);}, element, selected);
          first = false;
        }
      }
  
    }
    
    Tree::Tree(Parser &parser)
      : selection(parser) {}
  }
}