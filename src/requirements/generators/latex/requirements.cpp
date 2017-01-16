#include "generators/latex/requirements.hpp"

#include "annotations/sections.hpp"
#include "annotations/requirements.hpp"
#include "annotations/exception.hpp"

namespace generators {
  namespace latex {
    void printRequirements(::annotations::Sections& sections, ::annotations::Requirements& requirements, std::ostream& output) {
      auto filtered = sections.filter([&requirements](::requirements::Id id){
        return requirements.has(id);
      });
      for(auto& section: filtered) {
        switch(section.getDepth()) {
          case 0:
            output<<R"(\requirementssection{)"<<section.getTitle()<<"}"<<std::endl;
            break;
          case 1:
            output<<R"(\requirementssubsection{)"<<section.getTitle()<<"}"<<std::endl;
            break;
          default:
            output<<R"(\requirementsparagraph{)"<<section.getTitle()<<"}"<<std::endl;
            break;
        }
        output<<section.getDescription()<<std::endl;
        auto& elements = section.getElements();
        if(elements.size()!=0) {
          output<<R"(\requirementsbegintable)"<<std::endl;
          for (auto& element: elements) {
            auto& requirement = requirements.get(element);
            output<<R"(\requirementstableline{)"<<requirement.getRequirementKey()<<"}{"<<requirement.getText()<<"}"<<std::endl;
          }
          output<<R"(\requirementsendtable)"<<std::endl;
        }
      }
    }
  }
}