#include "req/console/printtree.hpp"

#include "requirements/node.hpp"
#include "requirements/id.hpp"
#include "requirements/icontent.hpp"

namespace req {
  namespace console {
    
    void printTree(std::ostream& stream, ::requirements::Node& node, const std::string& indent) {

      using namespace ::requirements;
      
      auto& content = node.getContent();
      if(&content!=nullptr) {
        stream<<"["<<id_to_string(node.getId())<<"]:";
        stream<<content.getDescription();
      } else {
        stream<<"Root node";
      }
      stream<<std::endl;
      auto& children = node.getChildren();
      auto it = children.begin();
      decltype(it) next;
      while(it!=children.end()) {
        stream<<indent;
        next = it;
        ++next;
        if(next!=children.end()) {
          stream<<" \xe2\x94\x9c\xe2\x94\x80";
          printTree(stream, **it, indent+" \xe2\x94\x82 ");
        } else {
          stream<<" \xe2\x94\x94\xe2\x94\x80";
          printTree(stream, **it, indent+"   ");
        }
        it=next;
      }
    }
    
    void printTree(std::ostream& stream, ::requirements::Node& node) {
      printTree(stream, node, "");
    }
    
  }
}