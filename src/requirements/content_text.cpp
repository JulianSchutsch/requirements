#include "requirements/content_text.hpp"

#include <istream>
#include <ostream>
#include <iterator>

#include "requirements/icontentvisitor.hpp"

namespace requirements {

bool Content_Text::isModified() {
  return modified;
}

void Content_Text::visit(IContentVisitor& visitor) {
  visitor.handleContent(*this);
}

void Content_Text::serialize(std::ostream& dest) {
  modified = false;
  dest<<content;
}

void Content_Text::deserialize(std::istream& source) {
  modified = false;
  content = std::string(std::istream_iterator<char>(source), {});
}

}