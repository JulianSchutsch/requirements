#pragma once

#include <string>

#include <boost/uuid/uuid.hpp>

namespace requirements {
  
  using Id = boost::uuids::uuid;
  
  Id generateRandomId();
  
  std::string id_to_string(const Id& id);
  bool string_to_id(const std::string& idStr, Id& id);
  
}