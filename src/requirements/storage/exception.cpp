#include "requirements/storage/exception.hpp"

namespace requirements {
  namespace storage {
    Exception::Exception(Reason a_reason)
      : reason(a_reason) {}

    const char* Exception::what() const noexcept {
      switch(reason) {
        case Reason::NotAFolder:
          return "Chosen path is not a folder";
        case Reason::CannotCreateFolder:
          return "Cannot create a folder at chosen path";
        case Reason::FailedToReadFolder:
          return "Could not read chosen path";
        case Reason::FolderNameEmpty:
          return "No folder given, empty";
        case Reason::IncompatibleRequirementId:
          return "Incompatible Requirement id";
        case Reason::CannotCreateRequirementsFolder:
          return "Cannot create requirements folder at chosen path";
        case Reason::InvalidId:
          return "Invalid id";
        case Reason::InvalidSuffix:
          return "Invalid file suffix";
        case Reason::InvalidRelationshipsFile:
          return "Invalid Relationships file";
        case Reason::InvalidTagsFile:
          return "Invalid Tags file";
      }
      return "Unknown ContextException issue";
    }
  }
}