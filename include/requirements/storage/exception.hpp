#pragma once

#include <exception>

namespace requirements {
  
  namespace storage {

    class Exception : public std::exception {
    public:
      enum class Reason {
        NotAFolder,
        CannotCreateFolder,
        FailedToReadFolder,
        FolderNameEmpty,
        CannotCreateRequirementsFolder,
        IncompatibleRequirementId,
        InvalidId,
        InvalidSuffix,
        InvalidRelationshipsFile,
        InvalidTagsFile,
        ReferenceToUnknownId,
        BlobNotFound
      };
    private:
      const Reason reason;
      
    public:
      Exception(Reason a_reason);
      const char* what() const noexcept override;
    };
    
  }
}