#include "generators/latex/requirements.hpp"

#include <iostream>
#include <fstream>
#include <map>
#include <stack>

#include <boost/algorithm/string.hpp>

#include "requirements/nodecollection.hpp"
#include "requirements/istorage.hpp"
#include "requirements/sectionparser.hpp"
#include "requirements/node.hpp"

namespace generators {
  namespace latex {

    namespace {

      class Context;

      using MacroFunction = void (*)(::requirements::NodePtr node, const std::string &section, Context& context);
      using MacroMap = std::map<std::string, MacroFunction>;
      using SimpleFunction = void (*)(Context& context);

      struct Context {
        ::requirements::IStorage &storage;
        int sectionPos;
        int sectionDepth = 0;
        std::ostream &file;
        std::string sectionPrefix;
        MacroMap *macros;
        SimpleFunction hasChildren = nullptr;
        SimpleFunction noChildren = nullptr;
        std::stack<SimpleFunction> postProcess;

        Context(::requirements::IStorage &a_storage, std::ostream &a_file, MacroMap &a_macros)
          : storage(a_storage), file(a_file), macros(&a_macros) {}
      };

      extern MacroMap macros_requirements;

      void macro_toplevel_text(::requirements::NodePtr node, const std::string& section, Context& context) {
        (void)node;
        context.file<<section<<std::endl;
      }

      void macro_toplevel_section(::requirements::NodePtr node, const std::string& section, Context& context) {
        switch(context.sectionDepth) {
          case 0:
            context.file << "\\requirementssection{" << section << "}" << std::endl;
            break;
          case 1:
            context.file << "\\requirementssubsection{" << section << "}" << std::endl;
            break;
          default:
            context.file << "\\requirementsparagraph{" << section << "}" <<std::endl;
        }
        context.file<<"\\label{req-sec:"<<::requirements::id_to_string(node->getId())<<"}"<<std::endl;
        ++context.sectionDepth;
      }

      void macro_toplevel_section_shortcut(::requirements::NodePtr node, const std::string& section, Context& context) {
        (void)node;
        context.sectionPrefix = boost::trim_copy(section);
      }

      void endchildren_requirements(Context& context) {
        context.file<<"\\requirementsendtable"<<std::endl;
      }

      void haschildren_requirements(Context& context) {
        context.file<<"\\requirementsbegintable"<<std::endl;
        context.postProcess.emplace(endchildren_requirements);
      }

      void macro_toplevel_style(::requirements::NodePtr node, const std::string& section, Context& context) {
        (void)node;
        std::string style = boost::trim_copy(section);
        if(style=="requirements") {
          context.hasChildren = haschildren_requirements;
          context.macros = &macros_requirements;
          return;
        }
        std::cout<<"Unknown style tag:"<<style<<std::endl;
      }

      void macro_requirements_text(::requirements::NodePtr node, const std::string& section, Context& context) {
        std::string key = context.sectionPrefix+std::to_string(context.sectionPos);
        context.file<<"\\requirementstableline{"<<key<<"}{"<<section<<"}"<<std::endl;
      }

      MacroMap macros_requirements = {
        {"text", macro_requirements_text}
      };

      MacroMap macros_topLevel = {
        {"text", macro_toplevel_text},
        {"section", macro_toplevel_section},
        {"section-shortcut", macro_toplevel_section_shortcut},
        {"style", macro_toplevel_style}
      };
    }

    static void printRequirementsNode(::requirements::NodePtr node, const Context& context) {
      Context childContext(context.storage, context.file, *context.macros);
      childContext.sectionPrefix = context.sectionPrefix;
      childContext.sectionPos = context.sectionPos;
      childContext.sectionDepth = context.sectionDepth;
      auto sections = ::requirements::parseSections(node->getContent());
      for(auto& section: sections) {
        auto it = context.macros->find(section.first);
        if(it==context.macros->end()) {
          std::cout<<"Skip section, not defined: "<<section.first<<std::endl;
          continue;
        }
        it->second(node, section.second, childContext);
      }
      auto& children = node->getChildren();
      if(children.empty()) {
        if(childContext.noChildren!=nullptr) {
          childContext.noChildren(childContext);
        }
      } else {
        if(childContext.hasChildren) {
          childContext.hasChildren(childContext);
        } else {
          childContext.sectionPrefix += std::to_string(childContext.sectionPos)+".";
        }
      }
      childContext.sectionPos = 0;
      for(auto& child: children) {
        ++childContext.sectionPos;
        printRequirementsNode(child, childContext);
      }
      while(!childContext.postProcess.empty()) {
        auto func = childContext.postProcess.top();
        childContext.postProcess.pop();
        func(childContext);
      }
    }

    void printRequirements(::requirements::NodeCollection &collection, ::requirements::IStorage &storage, const std::string &filename) {
      std::ofstream file(filename, std::fstream::out);
      Context context(storage, file, macros_topLevel);
      printRequirementsNode(collection.getRootNode(), context);
    }
  }
}