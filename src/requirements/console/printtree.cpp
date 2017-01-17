#include "requirements/console/printtree.hpp"

#include <sstream>

#include "boost/algorithm/string/split.hpp"
#include <boost/algorithm/string.hpp>

#include "requirements/console/console.hpp"

#include "requirements/select.hpp"

namespace requirements {
  namespace console {
    
    void printTree(PrintFunction p, const requirements::NodePtr& node, const std::string& firstIndent, const std::string& indent, const std::vector<requirements::NodePtr> selected) {
      using namespace ::requirements;
      using namespace ::requirements::console;
      auto& children = node->getChildren();
      if(node->getParent()) {
        if(requirements::isSelected(node, selected)) {
          p("["+firstIndent+bold()+id_to_string(node->getId())+reset()+"]");
        } else {
          p("["+firstIndent+id_to_string(node->getId())+"]");
        }
        std::vector<std::string> parts;
        boost::algorithm::split(parts, node->getContent(), boost::is_any_of("\n"));
        std::string textIndent;
        {
          std::stringstream ss;
          ss<<' '<<((children.begin()!=children.end())?line_vertical():" ")<<' ';
          textIndent = ss.str();
        }
        for(auto& part:parts) {
          p(indent+textIndent+" "+part);
        }
      } else {
        p("Root node");
      }
      auto it = children.begin();
      decltype(it) next;
      while(it!=children.end()) {
        next = it;
        ++next;
        std::string newFirstIndent  = indent + " " + ((next!=children.end())?line_verticalRight():line_bottomLeft()) + line_horizontal();
        std::string newIndent = indent + " " + ((next!=children.end())?line_vertical():" ") + " ";
        printTree(p, *it, newFirstIndent, newIndent, selected);
        it=next;
      }
    }
    
    void printTree(PrintFunction p, const ::requirements::NodePtr& node, const std::vector<requirements::NodePtr> selected) {
      printTree(p, node, "", "", selected);
    }
    
  }
}