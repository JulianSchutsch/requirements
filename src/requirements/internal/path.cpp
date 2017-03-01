#include "requirements/internal/path.hpp"

#include <boost/filesystem.hpp>
#include "requirements/exception.hpp"

namespace requirements {
  namespace internal {
    void ensureFolder(const std::string &subFolder) {
      if (!boost::filesystem::exists(subFolder)) {
        try {
          if (!boost::filesystem::create_directory(subFolder)) {
            throw Exception(Exception::Kind::User, "Failed to create folder %1%", {subFolder});
          }
        } catch (boost::filesystem::filesystem_error &e) {
          throw ::requirements::Exception(Exception::Kind::Other, "Failed to create folder %1%, exception %2%",
                                          {e.what(), subFolder});
        }
      } else {
        if (!boost::filesystem::is_directory(subFolder)) {
          throw ::requirements::Exception(Exception::Kind::User, "Cannot create folder %1%, already a file.",
                                          {subFolder});
        }
      }
    }
  }
}