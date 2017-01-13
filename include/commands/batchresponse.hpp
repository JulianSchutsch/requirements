#pragma once

#include "requirements/nodecollection.hpp"

#include "annotations/errors.hpp"
#include "annotations/shortcuts.hpp"
#include "annotations/sections.hpp"
#include "annotations/requirements.hpp"

namespace commands {
  struct BatchResponse final {
    std::unique_ptr<::requirements::NodeCollection> nodeCollection;
    std::unique_ptr<::annotations::Errors> errors;
    std::unique_ptr<::annotations::Shortcuts> shortcuts;
    std::unique_ptr<::annotations::Sections> sections;
    std::unique_ptr<::annotations::Requirements> requirements;
  };
}