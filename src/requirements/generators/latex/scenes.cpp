#include "requirements/generators/latex/scenes.hpp"

#include "requirements/generators/latex/sections.hpp"

namespace requirements {
  namespace generators {
    namespace latex {
      void printScenes(const annotations::Sections& sections,
                       const annotations::Scenes& scenes,
                       IPhaseFiles& files) {
        auto filtered = sections.filter([&scenes](::requirements::Id id) {
          return scenes.has(id);
        });
        iterSections(filtered, files,
                     [&scenes](const annotations::Section& section, std::ostream& output){
          auto &elements = section.getElements();
          if (elements.size() != 0) {
            for (auto &element: elements) {
              auto &scene = scenes.get(element);
              output << R"(\requirementsscene{)"<<scene.getKey()<<"}" << std::endl;
              output << scene.getText()<<std::endl;
            }
          }
        });
      }
    }
  }
}