#pragma once

#include <string>
#include <map>

#include "requirements/id.hpp"

namespace annotations {
  class Errors {
  private:
    using ErrorMap = std::map<::requirements::Id, std::string>;
    ErrorMap errors;
    friend class ErrorsBuilder;
  public:
    bool has(::requirements::Id id) const;
    const std::string& get(::requirements::Id id) const;
    ErrorMap::const_iterator cbegin() const { return errors.cbegin(); }
    ErrorMap::const_iterator cend() const { return errors.cend(); };
  };
  
  class ErrorsBuilder {
  private:
    Errors& errors;
  public:
    ErrorsBuilder(Errors& a_errors);
    void set(::requirements::Id id, const std::string& error);
  };
}