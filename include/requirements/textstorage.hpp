#pragma once

#include <exception>

#include "requirements/istorage.hpp"
#include "requirements/node.hpp"

namespace requirements {
  
  class TextStorage final : public IStorage {
  private:
    // Root-Path for the requirements
    std::string folder;
    std::string requirementsFolder; // Requirement-Folder, usually <folder>/requirements
    NodePtr rootNode; // Root-Node always exists

    void construct_readFolder();
    void construct_ensureFolder();
    
    void saveNode(Node* node);
  public:
    class ConstructException : public std::exception {
    public:
      enum class Reason {
        NotAFolder,
        CannotCreateFolder,
        FailedToReadFolder,
        FolderNameEmpty,
        CannotCreateRequirementsFolder,
        IncompatibleRequirementId
      };
    private:
      const Reason reason;
    public:
      ConstructException(Reason a_reason);
      const char* what() const noexcept override;
    };
    NodePtr createNode(std::unique_ptr<IContent>&& content) override;
    NodePtr getRootNode() noexcept override { return rootNode; }
    
    void save() override;
    
    // Throws ConstructException on failure
    explicit TextStorage(const std::string& a_folder);
  };
  
}