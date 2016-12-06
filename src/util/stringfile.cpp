#include "util/stringfile.hpp"

#include <fstream>
#include <sstream>

namespace util {
  
  std::string readFileToString(const std::string& filename) {
    std::ifstream f(filename.c_str());
    std::stringstream ss;
    ss<<f.rdbuf();
    return ss.str();
  }
  
}