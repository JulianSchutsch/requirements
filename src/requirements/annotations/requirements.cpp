#include "requirements/annotations/requirements.hpp"

#include "requirements/exception.hpp"

namespace requirements {
  namespace annotations {
    
    Requirement::Requirement(const std::string &a_key, const std::string &a_text)
      : key(a_key), text(a_text) {}
    
  }
}