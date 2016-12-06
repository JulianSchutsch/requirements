#include "console/console.hpp"

namespace console {
  std::string c_line_vertical{"\xe2\x94\x82"};
  std::string c_line_horizontal{"\xe2\x94\x80"};
  std::string c_line_verticalRight{"\xe2\x94\x9c"};
  std::string c_line_bottomLeft{"\xe2\x94\x94"};
  
  const std::string& line_vertical() { return c_line_vertical; }
  const std::string& line_horizontal() { return c_line_horizontal; }
  const std::string& line_verticalRight() { return c_line_verticalRight; }
  const std::string& line_bottomLeft() { return c_line_bottomLeft; }
}