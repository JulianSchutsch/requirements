#include "req/command/touch.hpp"

#include "requirements/storage/text.hpp"

#include "req/status.hpp"

namespace req{
  namespace command {
    void processCommand_touch(Status& status, const std::vector<std::string>& parameters) {
      (void)parameters;
      ::requirements::storage::Text storage(status.folder, true);
    }
  }
}