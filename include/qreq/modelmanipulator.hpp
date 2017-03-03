#pragma once

#include <functional>

#include "requirements/node.hpp"

#include <QModelIndex>
#include <QObject>

namespace qreq {

  class Model;

  class ModelManipulator : public QObject{
  Q_OBJECT
  private:
    Model& model;
    void forceNewChild(const QModelIndex& index,::requirements::NodePtr node);
    void forceNewSibling(const QModelIndex &index,::requirements::NodePtr node,bool copy_content=false);
  signals:
    void change_to_viewpos(const QModelIndex& index);
  public:
    void metaManipulator(const QModelIndex& index, std::function<void(::requirements::NodePtr)> generator);
    ModelManipulator(Model& model);
    void newSibling(const QModelIndex& index);
    void newTwin(const QModelIndex& index);
    void newChild(const QModelIndex& index);
    void deleteNode(const QModelIndex& index);
    void up(const QModelIndex& index);
    void down(const QModelIndex& index);
    void levelUp(const QModelIndex& index);
    void levelDown(const QModelIndex& index);
    void send_command(const std::string& command);
    void newBlob(const std::string& filename);
  };
}
