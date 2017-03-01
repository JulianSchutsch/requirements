#include "requirements/storage/text_common.hpp"

#include "requirements/internal/path.hpp"

namespace requirements {
  namespace storage {

    void text_ensureFolder(const std::string& folder) {
      ::requirements::internal::ensureFolder(folder);
      ::requirements::internal::ensureFolder(folder + text_requirementsFolder);
      ::requirements::internal::ensureFolder(folder + text_blobFolder);
      ::requirements::internal::ensureFolder(folder + text_latexFolder);
    }

  }
}