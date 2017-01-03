#include "annotations/parser.hpp"

#include "annotations/shortcut.hpp"
#include "annotations/error.hpp"

namespace annotations {
  
  namespace {
    struct Builders {
      SectionsBuilder sectionsBuilder;
      Builders(ParserResult& result)
        : sectionsBuilder(result.sections) {}
    };
  }
  
  static bool parseSections(::requirements::NodePtr, ParserResult& result, Builders& builders) {
    // In this case, we actually have to scan the text. How do we go about it?
    // The grammer is something like:
    // emptyLine*
    // section: Title \n
    // text*
    // For this purpose, we have to work with iterators
  }
  
  static bool parseTopLevel(::requirements::NodePtr node, ParserResult& result, Builders& builders) {
    // No text expected in root node
    // Check the children, check what you can know about them
    for(auto& child: node->getChildren()) {
      parseSections(child, result, builders);
    }
    return false;
  }
  
  bool parse(::requirements::NodePtr node, ParserResult& result) {
    result.shortcuts.clear();
    result.errors.clear();
    Builders builders(result);
    return parseTopLevel(node, result, builders);
  }
}