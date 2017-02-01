#include "qreq/modelmanipulator.hpp"

#include "qreq/model.hpp"

namespace qreq {

  void ModelManipulator::newSibling(const QModelIndex &index) {

  }

  void ModelManipulator::newChild(const QModelIndex &index) {

  }

  void ModelManipulator::up(const QModelIndex &index) {

  }

  void ModelManipulator::down(const QModelIndex& index) {

  }

  void ModelManipulator::deleteNode(const QModelIndex& index) {

  }

  void ModelManipulator::send_command(const std::string &command) {
    model.connector.send_command(command);
  }

  ModelManipulator::ModelManipulator(Model &a_model)
    : model(a_model){}

}