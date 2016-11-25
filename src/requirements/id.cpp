#include "requirements/id.hpp"

#include <sstream>

#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/lexical_cast.hpp>

namespace requirements {
  
  std::string id_to_string(const Id& id) {
    return boost::uuids::to_string(id);
  }

  bool string_to_id(const std::string& idStr, Id& id) {
    try {
      id = boost::lexical_cast<boost::uuids::uuid>(idStr);
      return true;
    } catch(boost::bad_lexical_cast& e) {
      return false;
    }
  }
  
  Id generateRandomId() {
    return boost::uuids::random_generator()();
  }
  
}