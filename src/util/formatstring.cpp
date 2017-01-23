#include "util/formatstring.hpp"

#include "boost/format.hpp"

namespace util {
  
  std::string formatString(const std::string& format, const std::vector<std::string>& parameters) {
    try {
      auto s = boost::format(format);
      for (auto &parameter: parameters) {
        s = s % parameter;
      }
      return s.str();
    }
    catch(...) {
    }
    return "(failed formatString:)"+format;
  }
  
}