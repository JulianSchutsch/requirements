#include "requirements/annotations/acceptances.hpp"

#include "requirements/exception.hpp"

namespace requirements {
  namespace annotations {
    Acceptance::Acceptance(const std::string& a_key, const std::string& a_text, const std::vector<::requirements::Id>& a_accepts)
      : key(a_key)
      , text(a_text)
      , accepts(a_accepts) {}
    
  }
}