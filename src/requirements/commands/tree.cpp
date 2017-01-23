#include "requirements/commands/tree.hpp"

#include "requirements/status.hpp"
#include "requirements/select.hpp"
#include "requirements/console/printtree.hpp"
#include "requirements/nodecollection.hpp"

namespace requirements {
  namespace commands {
    void Tree::execute(Status &status) {
      auto storage = status.openStorage();
  
      auto selected = selection.extractNodes(*storage);
      if(selected.size()==0) {
        selected = {storage->getNodeCollection().getRootNode()};
      }
  
      bool first = true;
      for(auto& element: selected) {
        if(!requirements::hasSelectedParent(element, selected)) {
          if(!first) {
            status.messageFunction(Status::MessageKind::Content, "----------------------------------", {});
          }
          console::printTree([&status](const std::string& msg){status.messageFunction(Status::MessageKind::Content, msg, {});}, element, selected);
          first = false;
        }
      }
  
    }
    
    Tree::Tree(Parser &parser)
      : selection(parser) {}
  }
}