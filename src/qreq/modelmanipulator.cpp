#include "qreq/modelmanipulator.hpp"

#include "requirements/commands/up.hpp"
#include "requirements/commands/down.hpp"
#include "requirements/commands/delete.hpp"
#include "requirements/commands/new.hpp"
#include "requirements/commands/nextto.hpp"
#include "requirements/commands/lastof.hpp"
#include "requirements/commands/levelup.hpp"
#include "requirements/commands/leveldown.hpp"
#include "requirements/commands/setcontent.hpp"

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
    metaManipulator(index, [this, index](::requirements::NodePtr node){
      forceNewSibling(index,node,false);
    });
  }

  void ModelManipulator::newTwin(const QModelIndex &index) {
    metaManipulator(index, [this, index](::requirements::NodePtr node){
      forceNewSibling(index,node,true);
    });
  }

  void ModelManipulator::forceNewSibling(const QModelIndex &index,::requirements::NodePtr node,bool copy_content){
    auto parentIndex = model.parent(index);
    auto newRow = node->childIndex()+1;

    model.beginInsertRows(parentIndex, newRow, newRow);
    auto newNode = model.model.nodeCollection->createNode("");
    newNode->setNextTo(node);
    model.endInsertRows();

    std::vector<std::unique_ptr<ICommand>> commands;
    commands.emplace_back(std::make_unique<::requirements::commands::New>(newNode->getId()));
    commands.emplace_back(std::make_unique<::requirements::commands::NextTo>(newNode->getId(), node->getId()));
    if(copy_content==true){
      commands.emplace_back(std::make_unique<::requirements::commands::SetContent>(newNode->getId(),node->getContent()));
    }
    model.connector._batchthread.enqueue(std::move(commands));
  }

  void ModelManipulator::newChild(const QModelIndex &index) {
    auto rootNode=model.model.nodeCollection->getRootNode();
    if(rootNode->getChildren().size()>0){
      metaManipulator(index, [this, index](::requirements::NodePtr node){
        forceNewChild(index,node);
      });
    }
    else{
      forceNewChild(index,rootNode);
    }
  }

  void ModelManipulator::forceNewChild(const QModelIndex &index,::requirements::NodePtr node){
    auto newRow = node->getChildren().size();

    model.beginInsertRows(index, newRow, newRow);
    auto newNode = model.model.nodeCollection->createNode("");
    newNode->setLastOf(node);
    model.endInsertRows();

    std::vector<std::unique_ptr<ICommand>> commands;
    commands.emplace_back(std::make_unique<::requirements::commands::New>(newNode->getId()));
    commands.emplace_back(std::make_unique<::requirements::commands::LastOf>(newNode->getId(), node->getId()));
    model.connector._batchthread.enqueue(std::move(commands));
  }

  void ModelManipulator::up(const QModelIndex &index) {
    metaManipulator(index, [this](::requirements::NodePtr node){model.connector._batchthread.enqueue(std::make_unique<::requirements::commands::Up>(node->getId())); });
  }

  void ModelManipulator::down(const QModelIndex& index) {
    metaManipulator(index, [this](::requirements::NodePtr node){model.connector._batchthread.enqueue(std::make_unique<::requirements::commands::Down>(node->getId())); });
  }

  void ModelManipulator::levelUp(const QModelIndex &index) {
    metaManipulator(index, [this](::requirements::NodePtr node){model.connector._batchthread.enqueue(std::make_unique<::requirements::commands::LevelUp>(node->getId())); });
  }

  void ModelManipulator::levelDown(const QModelIndex& index) {
    metaManipulator(index, [this](::requirements::NodePtr node){model.connector._batchthread.enqueue(std::make_unique<::requirements::commands::LevelDown>(node->getId())); });
  }

  void ModelManipulator::deleteNode(const QModelIndex& index) {
    metaManipulator(index, [this, index](::requirements::NodePtr node){

      auto parentIndex = model.parent(index);
      auto oldRow = node->childIndex();

      model.beginRemoveRows(parentIndex, oldRow, oldRow);
      model.model.nodeCollection->deleteNode(node);
      model.endRemoveRows();

      model.connector._batchthread.enqueue(std::make_unique<::requirements::commands::Delete>(node->getId()));
    });
  }

  void ModelManipulator::send_command(const std::string &command) {
    model.connector.send_command(command);
  }

  ModelManipulator::ModelManipulator(Model &a_model)
    : model(a_model){}

}
