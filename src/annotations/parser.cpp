#include "annotations/parser.hpp"

#include <boost/algorithm/string.hpp>

#include "util/lineparser.hpp"

#include "annotations/shortcut.hpp"

namespace annotations {
  
  namespace {
    struct Builders {
      SectionsBuilder sections;
      ErrorsBuilder errors;
      Builders(ParserResult& result)
        : sections(result.sections)
        , errors(result.errors) {}
    };
  }
  
  static bool parseSectionContext(::requirements::NodePtr node, ParserResult& result, Builders& builders);
  
  static bool parseSection(::requirements::NodePtr node, ParserResult& result, Builders& builders, util::LineParser& parser, const std::string& parameters) {
    bool success = true;
    builders.sections.enterSection(boost::algorithm::trim_copy(parameters), parser.consumeAll());
    for(auto& child: node->getChildren()) {
      success = success and parseSectionContext(node, result, builders);
    }
    builders.sections.leaveSection();
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
      {"section", parseSection}
    };
    auto tagIt = sectionContextTags.find(matches[1]);
    if(tagIt==sectionContextTags.end()) {
      builders.errors.set(node->getId(), "Unknown tag in section context");
      return false;
    }
    return tagIt->second(node, result, builders, parser, matches[2]);
  }
  
  static bool parseTopLevel(::requirements::NodePtr node, ParserResult& result, Builders& builders) {
    for(auto& child: node->getChildren()) {
      parseSectionContext(child, result, builders);
    }
    return false;
  }
  
  bool parse(::requirements::NodePtr node, ParserResult& result) {
    result.shortcuts.clear();
    Builders builders(result);
    return parseTopLevel(node, result, builders);
  }
}