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
      Builders(ParserResult& result)
        : sections(result.sections)
        , errors(result.errors)
        , shortcuts(result.shortcuts){}
    };
  }
  
  static bool parseSectionContext(::requirements::NodePtr node, ParserResult& result, Builders& builders);
  
  static bool parseRequirementsContext(::requirements::NodePtr node, ParserResult& result, Builders& builders) {
    // What do we expect? Any section style code? No... As this is a child, the entire content of this child is used for a new requirement
  }
  
  static bool parseRequirements(::requirements::NodePtr node, ParserResult& result, Builders& builders, util::LineParser& parser, const std::string& parameters) {
    static std::regex requirementsTitle(R"(\s*(\w+)\s*(.*))");
    std::smatch matches;
    if(!std::regex_match(parameters, matches, requirementsTitle)) {
      builders.errors.set(node->getId(), "A requirements title must be a shortcut followed by an arbitrary title string");
      return false;
    }
    SectionScope section(builders.sections, matches[2], parser.consumeAll());
    bool success = true;
    for(auto& child: node->getChildren()) {
      success = success and parseRequirementsContext(child, result, builders);
    }
    // TODO: Actually add requirements
    return success;
  }
  
  static bool parseSection(::requirements::NodePtr node, ParserResult& result, Builders& builders, util::LineParser& parser, const std::string& parameters) {
    bool success = true;
    auto title = boost::algorithm::trim_copy(parameters);
    builders.shortcuts.set(node->getId(), title);
    SectionScope section(builders.sections, title, parser.consumeAll());
    for(auto& child: node->getChildren()) {
      success = success and parseSectionContext(child, result, builders);
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
      {"requirements", parseRequirements}
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
      success = success and parseSectionContext(child, result, builders);
    }
    return success;
  }
  
  bool parse(::requirements::IStorage& storage, ParserResult& result) {
    Builders builders(result);
    return parseTopLevel(storage.getNodeCollection().getRootNode(), result, builders);
  }
}