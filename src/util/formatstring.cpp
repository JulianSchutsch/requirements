#include "util/formatstring.hpp"

#include "boost/format.hpp"

namespace util {
  
  bool formatString(const std::string& format, const std::vector<std::string>& parameters, std::string& result) {
    try {
      auto s = boost::format(format);
      for (auto &parameter: parameters) {
        s = s % parameter;
      }
      result = s.str();
    }
    catch(...) {
      return false;
    }
    return true;
  }
  
}