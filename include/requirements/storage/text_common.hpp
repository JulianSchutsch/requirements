#pragma once

#include <string>

namespace requirements {
  namespace storage {
    
    const std::string text_relationshipsFile = "relationships";
    const std::string text_blobaliasFile = "blobalias";
    const std::string text_trashNodeFile = "trashnode";
    const std::string text_requirementsFolder = "requirements/";
    const std::string text_blobFolder="blob/";
    const std::string text_latexFolder = "latex/";
    
    void text_ensureFolder(const std::string& folder);
  }
}