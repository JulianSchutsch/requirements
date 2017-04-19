#include "requirements/generators/latex/requirements.hpp"

#include "requirements/annotations/sections.hpp"
#include "requirements/annotations/requirements.hpp"
#include "requirements/exception.hpp"
#include "requirements/generators/latex/sections.hpp"

namespace requirements {
  namespace generators {
    namespace latex {
      void printRequirements(const annotations::Sections &sections,
                             const annotations::Requirements &requirements,
                             IPhaseFiles& files) {
        auto filtered = sections.filter([&requirements](::requirements::Id id) {
          return requirements.has(id);
        });
        iterSections(filtered, files,
                     [&requirements](const annotations::Section& section, std::ostream& output){
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
        });
      }
    }
  }
}