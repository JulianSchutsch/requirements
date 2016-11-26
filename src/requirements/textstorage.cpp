#include "requirements/textstorage.hpp"

#include <iostream>
#include <fstream>

#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>

#include "util/path.hpp"

#include "requirements/node.hpp"
#include "requirements/icontent.hpp"
#include "requirements/icontentvisitor.hpp"
#include "requirements/contentfactory.hpp"

namespace requirements {
  
  class ExtractContentSuffix : public IContentVisitor {
  private:
    std::string& suffix;
  public:
    void handleContent(Content_Text& text) override {
      (void)text;
      suffix = ".txt";
    }
    ExtractContentSuffix(std::string& a_suffix)
      : suffix(a_suffix) {}
  };

  void TextStorage::saveNode(Node& node) {
    std::string suffix;
    for(auto& entry: node.getChildren()) {
      auto& content = entry->getContent();
      if(content.isModified()) {
        content.visit(ExtractContentSuffix(suffix));
        std::fstream file(requirementsFolder+id_to_string(entry->getId())+suffix, std::fstream::out);
        content.serialize(file);
      }
      saveNode(*entry);
    }
  }
  
  void TextStorage::save() {
    saveNode(*rootNode);
  }
  
  NodePtr TextStorage::createNode(std::unique_ptr<IContent>&& content) {
    NodePtr newNode(new Node(*this, generateRandomId(), std::move(content)));
    newNode->setParent(rootNode);
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
  }

  void TextStorage::construct_readFolder() {
    try {
      for(auto it=boost::filesystem::directory_iterator(requirementsFolder);it!=boost::filesystem::directory_iterator();++it) {
        boost::filesystem::path path(*it);
        Id id;
        if(!string_to_id(path.stem().string(), id)) {
          throw ConstructException(ConstructException::Reason::InvalidId);
        }
        const std::string& extensionString = path.extension().string();
        const std::string suffix(extensionString, 1, extensionString.size()-1);
        auto content = contentFactory(suffix);
        if(!content) {
          throw ConstructException(ConstructException::Reason::InvalidSuffix);
        }
        auto node = new Node(*this, id, std::move(content));
        node->setParent(rootNode);
      }
    }
    catch(boost::filesystem::filesystem_error& e) {
      throw ConstructException(ConstructException::Reason::FailedToReadFolder);
    }
  }
  
  TextStorage::TextStorage(const std::string& a_folder)
    : folder(util::ensureTrailingSlash(a_folder))
    , requirementsFolder(folder+"requirements/")
    , rootNode(new Node(*this, generateRandomId(), nullptr)) {
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
      case Reason::CannotCreateRequirementsFolder:
        return "Cannot create requirements folder at chosen path";
      case Reason::InvalidId:
        return "Invalid id";
      case Reason::InvalidSuffix:
        return "Invalid file suffix";
    }
    return "Unknown ContextException issue";
  }

}