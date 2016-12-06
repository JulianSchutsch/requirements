#include "req/console/printtree.hpp"

#include <sstream>

#include "boost/algorithm/string/split.hpp"
#include <boost/algorithm/string.hpp>

#include "console/console.hpp"

#include "requirements/id.hpp"

namespace req {
  namespace console {
    
    void printTree(std::ostream& stream, const requirements::NodePtr& node, const std::string& indent, const std::vector<requirements::NodePtr> selected) {
      using namespace ::requirements;
      auto& children = node->getChildren();
      if(node->getParent()) {
        stream<<"["<<id_to_string(node->getId())<<"]"<<std::endl;
        std::vector<std::string> parts;
        boost::algorithm::split(parts, node->getContent(), boost::is_any_of("\n"));
        std::string textIndent;
        {
          std::stringstream ss;
          ss<<' '<<((children.begin()!=children.end())?::console::line_verticalRight():" ")<<' ';
          textIndent = ss.str();
        }
        for(auto& part:parts) {
          stream<<indent<<textIndent<<" "<<part<<std::endl;
        }
      } else {
        stream<<"Root node"<<std::endl;
      }
      auto it = children.begin();
      decltype(it) next;
      while(it!=children.end()) {
        stream<<indent;
        next = it;
        ++next;
        if(next!=children.end()) {
          stream<<" \xe2\x94\x9c\xe2\x94\x80";
          printTree(stream, *it, indent+" \xe2\x94\x82 ", selected);
        } else {
          stream<<" \xe2\x94\x94\xe2\x94\x80";
          printTree(stream, *it, indent+"   ", selected);
        }
        it=next;
      }
    }
    
    void printTree(std::ostream& stream, const ::requirements::NodePtr& node, const std::vector<requirements::NodePtr> selected) {
      printTree(stream, node, "", selected);
    }
    
  }
}