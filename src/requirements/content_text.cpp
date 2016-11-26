#include "requirements/content_text.hpp"

#include <istream>
#include <ostream>
#include <iterator>

#include "requirements/icontentvisitor.hpp"

namespace requirements {

void Content_Text::visit(IContentVisitor& visitor) {
  visitor.handleContent(*this);
}

void Content_Text::serialize(std::ostream& dest) {
  dest<<content;
}

void Content_Text::deserialize(std::istream& source) {
  content = std::string(std::istream_iterator<char>(source), {});
}

}