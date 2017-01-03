
#pragma once

#include <string>
#include <regex>

namespace util {
  
  class LineParser {
  private:
    std::string content;
    std::string::const_iterator position;
  public:
    LineParser(const std::string& a_content);
    bool consume(const std::regex& regex, std::smatch& matches);
    bool consume(std::string& line);
    bool skipEmptyLine();
  };
}