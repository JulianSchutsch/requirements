#include "requirements/storage/text_common.hpp"

#include <boost/filesystem.hpp>

#include "requirements/storage/exception.hpp"

namespace requirements {
  namespace storage {
    
    void text_ensureFolder(const std::string& folder) {
      // Ensure root folder exists
      if(!boost::filesystem::exists(folder)) {
        if(!boost::filesystem::create_directory(folder)) {
          throw Exception(Exception::Reason::CannotCreateFolder);
        }
      } else {
        if(!boost::filesystem::is_directory(folder)) {
          throw Exception(Exception::Reason::NotAFolder);
        }
      }
    }

    void text_ensureFolders(const std::string& folder, const std::string& requirementsFolder) {
      text_ensureFolder(folder);

      // Ensure requirements subfolder exists
      if(!boost::filesystem::exists(requirementsFolder)) {
        if(!boost::filesystem::create_directory(requirementsFolder)) {
          throw Exception(Exception::Reason::CannotCreateFolder);
        }
      } else {
        if(!boost::filesystem::is_directory(folder)) {
          throw Exception(Exception::Reason::NotAFolder);
        }
      }
    }

  }
}