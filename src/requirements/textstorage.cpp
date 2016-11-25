#include "requirements/textstorage.hpp"

#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>

#include "util/path.hpp"

#include "requirements/node.hpp"

namespace requirements {
  
  void TextStorage::save() {
  }
  
  std::shared_ptr<Node> TextStorage::createNode() {
    std::shared_ptr<Node> newNode(new Node(generateRandomId()));
    return newNode;
  }
  
  void TextStorage::construct_ensureFolder() {
    // Ensure root folder exists
    if(!boost::filesystem::exists(folder)) {
      if(!boost::filesystem::create_directory(folder)) {
        throw ConstructException(ConstructException::Reason::CannotCreateFolder);
      }
    } else {
      if(!boost::filesystem::is_directory(folder)) {
        throw ConstructException(ConstructException::Reason::NotAFolder);
      }
    }

    // Ensure requirements subfolder exists
    if(!boost::filesystem::exists(requirementsFolder)) {
      if(!boost::filesystem::create_directory(requirementsFolder)) {
        throw ConstructException(ConstructException::Reason::CannotCreateFolder);
      }
    } else {
      if(!boost::filesystem::is_directory(folder)) {
        throw ConstructException(ConstructException::Reason::NotAFolder);
      }
    }
  };

  void TextStorage::construct_readFolder() {
    try {
      for(auto it=boost::filesystem::directory_iterator(requirementsFolder);it!=boost::filesystem::directory_iterator();++it) {
        std::cout<<*it<<std::endl;
      }
    }
    catch(boost::filesystem::filesystem_error& e) {
      throw ConstructException(ConstructException::Reason::FailedToReadFolder);
    }
  }
  
  TextStorage::TextStorage(const std::string& a_folder)
    : folder(util::ensureTrailingSlash(a_folder))
    , requirementsFolder(folder+"requirements/")
    , rootNode(new Node(generateRandomId())) {
    if(folder.empty()) {
      throw ConstructException(ConstructException::Reason::FolderNameEmpty);
    }
    construct_ensureFolder();
    construct_readFolder();  
  }
  
  TextStorage::ConstructException::ConstructException(Reason a_reason)
    : reason(a_reason) {}

  const char* TextStorage::ConstructException::what() const noexcept {
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
    }
  }

}