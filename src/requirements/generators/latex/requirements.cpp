#include "requirements/generators/latex/requirements.hpp"

#include "requirements/annotations/sections.hpp"
#include "requirements/annotations/requirements.hpp"
#include "requirements/exception.hpp"

namespace requirements {
  namespace generators {
    namespace latex {
      void printRequirements(annotations::Sections &sections,
                             annotations::Requirements &requirements,
                             IPhaseFiles& files) {
        auto filtered = sections.filter([&requirements](::requirements::Id id) {
          return requirements.has(id);
        });
        for (auto &section: filtered) {
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
          auto &elements = section.getElements();
          if (elements.size() != 0) {
            output << R"(\requirementsbegintable)" << std::endl;
            for (auto &element: elements) {
              auto &requirement = requirements.get(element);
              output << R"(\requirementstableline{)" << requirement.getKey() << "}{" << requirement.getText()
                     << "}" << std::endl;
            }
            output << R"(\requirementsendtable)" << std::endl;
          }
        }
      }
    }
  }
}