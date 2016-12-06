#include "req/console/printtree.hpp"

#include <sstream>

#include "boost/algorithm/string/split.hpp"
#include <boost/algorithm/string.hpp>

#include "console/console.hpp"

#include "requirements/id.hpp"
#include "requirements/select.hpp"

namespace req {
  namespace console {
    
    void printTree(std::ostream& stream, const requirements::NodePtr& node, const std::string& indent, const std::vector<requirements::NodePtr> selected) {
      using namespace ::console;
      using namespace ::requirements;
      auto& children = node->getChildren();
      if(node->getParent()) {
        stream<<"[";
        if(requirements::isSelected(node, selected)) {
          stream<<bold()<<id_to_string(node->getId())<<reset();
        } else {
          stream<<id_to_string(node->getId());
        }
        stream<<"]"<<std::endl;
        std::vector<std::string> parts;
        boost::algorithm::split(parts, node->getContent(), boost::is_any_of("\n"));
        std::string textIndent;
        {
          std::stringstream ss;
          ss<<' '<<((children.begin()!=children.end())?line_vertical():" ")<<' ';
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
          stream<<' '<<line_verticalRight()<<line_horizontal();
          printTree(stream, *it, indent+' '+line_vertical()+' ', selected);
        } else {
          stream<<' '<<line_bottomLeft()<<line_horizontal();
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