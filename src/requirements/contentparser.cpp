#include "requirements/contentparser.hpp"

#include <sstream>

#include "requirements/icontent.hpp"
#include "requirements/content/text.hpp"

namespace requirements {
  
  static bool internal_checkSection(std::string::const_iterator& ppos, std::string::const_iterator end, std::string& sectionName) {
    auto start = ppos;
    auto pos =ppos;
    while(pos!=end) {
      if((*pos<'a') || (*pos>'z')) {
        break;
      }
      ++pos;
    }
    if(pos==end) {
      return false;
    }
    if(*pos!=':') {
      return false;
    }
    sectionName = std::string(start, pos);
    pos++;
    ppos = pos;
    return true;
  }
  
  static void internal_consumeLine(std::string::const_iterator& ppos, std::string::const_iterator end, std::stringstream& ss) {
    auto start = ppos;
    while(ppos!=end && *ppos!='\n') {
    }
    if(*ppos=='\n') {
      ++ppos;
    }
    ss<<std::string(start, ppos);
  }
  
  static void internal_createContent(const std::string& sectionName, const std::stringstream& section, std::vector<std::unique_ptr<IContent>>& results) {
    results.emplace_back(new content::Text(section.str()));
  }
  
  static void internal_parseContent(const std::string& str, std::vector<std::unique_ptr<IContent>>& results) {
    auto pos = str.cbegin();
    auto end = str.cend();
    std::stringstream section;
    std::string sectionName;
    if(!internal_checkSection(pos, end, sectionName)) {
      sectionName = "text";
    }
    while(pos!=end) {
      internal_consumeLine(pos, end, section);
      std::string newSectionName;
      if(internal_checkSection(pos, end, newSectionName)) {
        internal_createContent(sectionName, section, results);
        sectionName = newSectionName;
      }
    }
    internal_createContent(sectionName, section, results);
  }

  std::vector<std::unique_ptr<IContent>> parseAnnotations(NodePtr node) {
    std::vector<std::unique_ptr<IContent>> result;
    internal_parseContent(node->getAnnotations(), result);
    return std::move(result);
  }
  
  std::vector<std::unique_ptr<IContent>> parseContent(NodePtr node) {
    std::vector<std::unique_ptr<IContent>> result;
    internal_parseContent(node->getContent(), result);
    return std::move(result);
  }
  
}