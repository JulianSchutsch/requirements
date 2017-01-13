#include "annotations/parser.hpp"

#include <boost/algorithm/string.hpp>

#include "util/lineparser.hpp"

#include "requirements/node.hpp"
#include "requirements/nodecollection.hpp"

namespace annotations {
  
  namespace {
    struct Builders {
      SectionsBuilder sections;
      ErrorsBuilder errors;
      ShortcutsBuilder shortcuts;
      RequirementsBuilder requirements;
      Builders(ParserResult& result)
        : sections(*result.sections)
        , errors(*result.errors)
        , shortcuts(*result.shortcuts)
        , requirements(*result.requirements) {}
    };
  }
  
  static bool parseSectionContext(::requirements::NodePtr node, ParserResult& result, Builders& builders);
  
  static bool parseRequirement(::requirements::NodePtr node, ParserResult& result, Builders& builders) {
    RequirementsBuilderScope scope(builders.requirements, node->getId(), node->getContent());
    builders.sections.addElement(node->getId());
    builders.shortcuts.set(node->getId(), scope.getKey());
    bool success = true;
    for(auto& child: node->getChildren()) {
      success = success && parseRequirement(child, result, builders);
    }
    return success;
  }
  
  static bool parseRequirementSection(::requirements::NodePtr node, ParserResult& result, Builders& builders, util::LineParser& parser, const std::string& parameters) {
    static std::regex requirementsTitle(R"(\s*(\w+)\s*(.*))");
    std::smatch matches;
    if(!std::regex_match(parameters, matches, requirementsTitle)) {
      builders.errors.set(node->getId(), "A requirements title must be a shortcut followed by an arbitrary title string");
      return false;
    }
    const std::string& shortcut = matches[1];
    const std::string& title = matches[2];
    
    builders.shortcuts.set(node->getId(), title);
    builders.requirements.setMajorPrefix(shortcut);
    
    SectionsBuilderScope section(builders.sections, title, parser.consumeAll());
    
    bool success = true;
    for(auto& child: node->getChildren()) {
      success = success && parseRequirement(child, result, builders);
    }
    return success;
  }
  
  static bool parseSection(::requirements::NodePtr node, ParserResult& result, Builders& builders, util::LineParser& parser, const std::string& parameters) {
    bool success = true;
    auto title = boost::algorithm::trim_copy(parameters);
    builders.shortcuts.set(node->getId(), title);
    SectionsBuilderScope section(builders.sections, title, parser.consumeAll());
    for(auto& child: node->getChildren()) {
      success = success && parseSectionContext(child, result, builders);
    }
    return success;
  }
  
  static bool parseSectionContext(::requirements::NodePtr node, ParserResult& result, Builders& builders) {
    util::LineParser parser(node->getContent());
    while(parser.skipEmptyLine()) {}
    std::smatch matches;
    static std::regex sectionRegex(R"((\w+):\s*(.*))");
    if(!parser.consume(sectionRegex, matches)) {
      builders.errors.set(node->getId(), "First non empty line must be a \"tag:\"");
      return false;
    }
    
    static std::map<std::string, bool (*)(::requirements::NodePtr node, ParserResult& result, Builders& builders, util::LineParser& parser, const std::string& parameters)> sectionContextTags = {
      {"section", parseSection},
      {"requirements", parseRequirementSection}
    };
    auto tagIt = sectionContextTags.find(matches[1]);
    if(tagIt==sectionContextTags.end()) {
      builders.errors.set(node->getId(), "Unknown tag in section context, "+std::string(matches[1]));
      return false;
    }
    return tagIt->second(node, result, builders, parser, matches[2]);
  }
  
  static bool parseTopLevel(::requirements::NodePtr node, ParserResult& result, Builders& builders) {
    bool success = true;
    for(auto& child: node->getChildren()) {
      success = success && parseSectionContext(child, result, builders);
    }
    return success;
  }
  
  bool parse(::requirements::IStorage& storage, ParserResult& result) {
    result.errors.reset(new Errors());
    Builders builders(result);
    return parseTopLevel(storage.getNodeCollection().getRootNode(), result, builders);
  }
}