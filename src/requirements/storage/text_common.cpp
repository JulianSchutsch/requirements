#include "requirements/storage/text_common.hpp"

#include <boost/filesystem.hpp>

#include "requirements/storage/exception.hpp"

namespace requirements {
  namespace storage {

    static void ensureSubFolder(const std::string& subFolder) {
      if(!boost::filesystem::exists(subFolder)) {
        if(!boost::filesystem::create_directory(subFolder)) {
          throw Exception(Exception::Reason::CannotCreateFolder);
        }
      } else {
        if(!boost::filesystem::is_directory(subFolder)) {
          throw Exception(Exception::Reason::NotAFolder);
        }
      }
    }
    
    void text_ensureFolder(const std::string& folder) {
      // Ensure root folder exists
      ensureSubFolder(folder);
      ensureSubFolder(folder + text_requirementsFolder);
      ensureSubFolder(folder + text_blobFolder);
      ensureSubFolder(folder + text_latexFolder);
    }

  }
}