#pragma once

#include <ostream>

namespace requirements {
  class Node;
}

namespace req {
  namespace console {
  
    void printTree(std::ostream& stream, ::requirements::Node& node);

  }
}