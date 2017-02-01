#pragma once

#include <QModelIndex>

namespace qreq {

  class Model;

  class ModelManipulator {
  private:
    Model& model;
  public:
    ModelManipulator(Model& model);
    void newSibling(const QModelIndex& index);
    void newChild(const QModelIndex& index);
    void deleteNode(const QModelIndex& index);
    void up(const QModelIndex& index);
    void down(const QModelIndex& index);
    void send_command(const std::string& command);
  };
}