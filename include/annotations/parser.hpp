#pragma once

#include <map>

#include "requirements/id.hpp"
#include "requirements/node.hpp"

#include "annotations/sections.hpp"

namespace annotations {
  
  class ShortCut;
  class Error;
  
  struct ParserResult {
    std::map<::requirements::Id, ShortCut> shortcuts;
    std::map<::requirements::Id, Error> errors;
    Sections sections;
  };
  
  bool parse(::requirements::NodePtr node, ParserResult& result);
}