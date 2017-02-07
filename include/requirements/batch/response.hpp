#pragma once

#include "requirements/nodecollection.hpp"

#include "requirements/annotations/errors.hpp"
#include "requirements/annotations/shortcuts.hpp"
#include "requirements/annotations/sections.hpp"
#include "requirements/annotations/requirements.hpp"
#include "requirements/annotations/acceptances.hpp"

#include "requirements/status.hpp"

namespace requirements {
  namespace batch {
    struct Response final {
      std::unique_ptr<::requirements::Status> status;
      std::unique_ptr<::requirements::NodeCollection> nodeCollection;
      std::unique_ptr<::requirements::annotations::Errors> errors;
      std::unique_ptr<::requirements::annotations::Shortcuts> shortcuts;
      std::unique_ptr<::requirements::annotations::Sections> sections;
      std::unique_ptr<::requirements::annotations::Requirements> requirements;
      std::unique_ptr<::requirements::annotations::Acceptances> acceptances;
    };
  }
}