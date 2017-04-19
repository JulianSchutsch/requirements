#include "requirements/generators/latex/sections.hpp"
#include <fstream>

#include "requirements/generators/iphasefiles.hpp"

namespace requirements {
  namespace generators {
    namespace latex {
      void iterSections(annotations::Sections& sections,
                        IPhaseFiles& files,
                        std::function<void(const annotations::Section&, std::ostream&)> partGenerator) {
        for (auto &section: sections) {
          auto& output = files.getOStream(section.getPhaseIdentifier());
          switch (section.getDepth()) {
            case 0:
              output << R"(\requirementssection{)" << section.getTitle() << "}" << std::endl;
              break;
            case 1:
              output << R"(\requirementssubsection{)" << section.getTitle() << "}" << std::endl;
              break;
            default:
              output << R"(\requirementsparagraph{)" << section.getTitle() << "}" << std::endl;
              break;
          }
          output << section.getDescription() << std::endl;
          partGenerator(section, output);
        }
      }
    }
  }
}