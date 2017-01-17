#include "requirements/commands/touch.hpp"

#include "requirements/status.hpp"

namespace requirements {
  namespace commands {
    void Touch::execute(Status &status) {
      auto storage = status.openStorage();
      (void)storage;
    }
    
    Touch::Touch(Parser &parser) {(void)parser;}
  }
}