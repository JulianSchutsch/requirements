#pragma once

#include <QWidget>

namespace qreq {

  class Actions {
  public:
    Actions(QWidget* a_parent);
  private:
    using Sequence = std::list<std::string>;
    using Sequences = std::map<std::string, Sequence>;
    QWidget* parent;
  };

}