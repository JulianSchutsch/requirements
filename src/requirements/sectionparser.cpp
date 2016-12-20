#include "requirements/sectionparser.hpp"

#include <sstream>
#include <set>

namespace requirements {
  
  static bool internal_checkSection(std::string::const_iterator& ppos, std::string::const_iterator end, std::string& sectionName) {
    auto start = ppos;
    auto pos = ppos;
    while(pos!=end) {
      if(((*pos<'a') || (*pos>'z')) && (*pos!='-')) {
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
    ++pos;
    ppos = pos;
    return true;
  }
  
  static void internal_consumeLine(std::string::const_iterator& ppos, std::string::const_iterator end, std::stringstream& ss) {
    auto start = ppos;
    while(ppos!=end && *ppos!='\n') {
      ++ppos;
    }
    ss<<std::string(start, ppos);
    if(*ppos=='\n') {
      ++ppos;
    }
  }

  static std::set<std::string> multilineSections = {"text"};

  static void internal_parseContent(const std::string& str, Sections& results) {
    auto pos = str.cbegin();
    auto end = str.cend();
    while(pos!=end) {
      std::stringstream section;
      std::string sectionName;
      if(!internal_checkSection(pos, end, sectionName)) {
        sectionName = "text";
      }
      bool multiLine = multilineSections.find(sectionName)!=multilineSections.end();
      internal_consumeLine(pos, end, section);
      if(multiLine) {
        while(pos!=end) {
          if (internal_checkSection(pos, end, sectionName)) {
            break;
          }
          section<<std::endl;
          internal_consumeLine(pos, end, section);
        }
      }
      results.emplace_back(std::make_pair(std::move(sectionName), section.str()));
    }
  }
  
  Sections parseSections(const std::string& content) {
    Sections result;
    internal_parseContent(content, result);
    return std::move(result);
  }
  
}