#pragma once

#include <string>

namespace requirements {
  namespace storage {
    
    const std::string text_relationshipsFile = "relationships";
    const std::string text_requirementsFolder = "requirements/";
    const std::string text_annotationsFolder="annotations/";
    
    void text_ensureFolder(const std::string& folder);
  }
}