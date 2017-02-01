#include "qreq/modelmanipulator.hpp"

#include "requirements/commands/up.hpp"
#include "requirements/commands/down.hpp"
#include "requirements/commands/delete.hpp"
#include "requirements/commands/new.hpp"
#include "requirements/commands/nextto.hpp"
#include "requirements/commands/lastof.hpp"

#include "qreq/model.hpp"

namespace qreq {

  void ModelManipulator::metaManipulator(const QModelIndex& index, std::function<void(::requirements::NodePtr)> generator) {
    if(!index.isValid()) {
      return;
    }
    auto node = model.getNodeFromModelIndex(index);
    if(!node) {
      return;
    }
    generator(node);
  }

  void ModelManipulator::newSibling(const QModelIndex &index) {
    metaManipulator(index, [this](::requirements::NodePtr node){
      auto newId = ::requirements::generateRandomId();
      std::vector<std::unique_ptr<ICommand>> commands;
      commands.emplace_back(std::make_unique<::requirements::commands::New>(newId));
      commands.emplace_back(std::make_unique<::requirements::commands::NextTo>(newId, node->getId()));
      model.connector._batchthread.enqueue(std::move(commands));
    });
  }

  void ModelManipulator::newChild(const QModelIndex &index) {
    metaManipulator(index, [this](::requirements::NodePtr node){
      auto newId = ::requirements::generateRandomId();
      std::vector<std::unique_ptr<ICommand>> commands;
      commands.emplace_back(std::make_unique<::requirements::commands::New>(newId));
      commands.emplace_back(std::make_unique<::requirements::commands::LastOf>(newId, node->getId()));
      model.connector._batchthread.enqueue(std::move(commands));
    });
  }

  void ModelManipulator::up(const QModelIndex &index) {
    metaManipulator(index, [this](::requirements::NodePtr node){model.connector._batchthread.enqueue(std::make_unique<::requirements::commands::Up>(node->getId())); });
  }

  void ModelManipulator::down(const QModelIndex& index) {
    metaManipulator(index, [this](::requirements::NodePtr node){model.connector._batchthread.enqueue(std::make_unique<::requirements::commands::Down>(node->getId())); });
  }

  void ModelManipulator::deleteNode(const QModelIndex& index) {
    metaManipulator(index, [this](::requirements::NodePtr node){model.connector._batchthread.enqueue(std::make_unique<::requirements::commands::Delete>(node->getId())); });
  }

  void ModelManipulator::send_command(const std::string &command) {
    model.connector.send_command(command);
  }

  ModelManipulator::ModelManipulator(Model &a_model)
    : model(a_model){}

}