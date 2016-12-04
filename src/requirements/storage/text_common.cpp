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

      const std::string requirementsFolder = folder + text_requirementsFolder;
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
      
      const std::string annotationsFolder = folder + text_annotationsFolder;
      if(!boost::filesystem::exists(annotationsFolder)) {
        if(!boost::filesystem::create_directory(annotationsFolder)) {
          throw Exception(Exception::Reason::CannotCreateFolder);
        }
      } else {
        if(!boost::filesystem::is_directory(annotationsFolder)) {
          throw Exception(Exception::Reason::NotAFolder);
        }
      }
      
    }

  }
}