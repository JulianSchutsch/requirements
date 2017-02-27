#include "requirements/generators/latex/labels.hpp"

#include "requirements/exception.hpp"

namespace requirements {
  namespace generators {
    namespace latex {
      void printLabels(NodeCollection& collection,
                       annotations::Shortcuts& shortcuts,
                       std::ostream& output) {
        for(auto& node: collection) {
          auto id = node->getId();
          output << R"(\requirementslabel{)"<<id_to_string(id)<<"}{";
          if(shortcuts.has(id)) {
            output<<shortcuts.get(id);
          } else {
            output<<id_to_string(id);
          }
          output<<"}"<<std::endl;
        }
      }
    }
  }
}