#include "annotations/errors.hpp"

namespace annotations {
  
  ErrorsBuilder::ErrorsBuilder(Errors& a_errors)
    : errors(a_errors) { errors.errors.clear(); }
  
  void ErrorsBuilder::set(::requirements::Id id, const std::string& error) {
    errors.errors[id] = error;
  }
  
  const std::string& Errors::get(::requirements::Id id) const {
    return errors.at(id);
  }
  
  bool Errors::has(::requirements::Id id) const {
    return errors.find(id)!=errors.end();
  }
}