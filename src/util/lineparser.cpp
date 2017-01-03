#include "util/lineparser.hpp"

namespace util {
  
  LineParser::LineParser(const std::string& a_content)
    : content(a_content)
    , position(content.begin()) {}
  
  bool LineParser::consume(const std::regex& regex, std::smatch& matches) {
    // Find end of line
    auto eol = position;
    while(eol!=content.end() && *eol!='\n') {
      ++eol;
    }
    if(!std::regex_match(position, eol, matches, regex)) {
      return false;
    }
    position = eol;
    if(position!=content.end()) {
      ++position;
    }
    return true;
  }
  
  bool LineParser::consume(std::string& line) {
    if(position==content.end()) {
      return false;
    }
    auto eol = position;
    while(eol!=content.end() && *eol!='\n') {
      ++eol;
    }
    line = std::string(position, eol);
    position = eol;
    if(position!=content.end()) {
      ++position;
    }
    return true;
  }
  
}