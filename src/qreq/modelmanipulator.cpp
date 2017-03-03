#include "qreq/modelmanipulator.hpp"

#include "requirements/commands/up.hpp"
#include "requirements/commands/down.hpp"
#include "requirements/commands/trash.hpp"
#include "requirements/commands/new.hpp"
#include "requirements/commands/nextto.hpp"
#include "requirements/commands/lastof.hpp"
#include "requirements/commands/levelup.hpp"
#include "requirements/commands/leveldown.hpp"
#include "requirements/commands/setcontent.hpp"
#include "requirements/commands/newblob.hpp"

#include "qreq/model.hpp"

#include <iostream>

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
    auto newNode = model.model.nodeCollection->createNode(copy_content?node->getContent():"");
    newNode->setNextTo(node);
    QModelIndex newIndex=model.index(newRow,0,parentIndex);
    model.endInsertRows();

    std::vector<std::unique_ptr<ICommand>> commands;
    commands.emplace_back(std::make_unique<::requirements::commands::New>(newNode->getId()));
    commands.emplace_back(std::make_unique<::requirements::commands::NextTo>(newNode->getId(), node->getId()));
    if(copy_content==true){
      commands.emplace_back(std::make_unique<::requirements::commands::SetContent>(newNode->getId(),node->getContent()));
    }
    model.connector._batchthread.enqueue(std::move(commands));
    emit change_to_viewpos(newIndex);
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
    QModelIndex newIndex=model.index(newRow,0,index);
    model.endInsertRows();

    std::vector<std::unique_ptr<ICommand>> commands;
    commands.emplace_back(std::make_unique<::requirements::commands::New>(newNode->getId()));
    commands.emplace_back(std::make_unique<::requirements::commands::LastOf>(newNode->getId(), node->getId()));
    model.connector._batchthread.enqueue(std::move(commands));
    emit change_to_viewpos(newIndex);
  }

  void ModelManipulator::up(const QModelIndex &index) {
    metaManipulator(index, [this, index](::requirements::NodePtr node){
      auto oldIndex = node->childIndex();
      if(oldIndex==0) {
        return;
      }
      auto parent = model.parent(index);
      auto newIndex = oldIndex-1;
      model.beginMoveRows(parent, oldIndex, oldIndex, parent, newIndex);
      node->up();
      model.endMoveRows();
      emit change_to_viewpos(model.index(newIndex,0, parent));
      model.connector._batchthread.enqueue(std::make_unique<::requirements::commands::Up>(node->getId()));
    });
  }

  void ModelManipulator::down(const QModelIndex& index) {
    metaManipulator(index, [this, index](::requirements::NodePtr node){
      auto oldIndex= node->childIndex();
      auto parentNode = node->getParent();
      if((size_t)oldIndex==parentNode->getChildren().size()-1) {
        return;
      }
      auto parent = model.parent(index);
      auto newIndex = oldIndex+1;
      model.beginMoveRows(parent, oldIndex, oldIndex, parent, newIndex+1);
      node->down();
      model.endMoveRows();
      emit change_to_viewpos(model.index(newIndex,0, parent));
      model.connector._batchthread.enqueue(std::make_unique<::requirements::commands::Down>(node->getId()));
    });
  }

  void ModelManipulator::levelUp(const QModelIndex &index) {
    metaManipulator(index, [this, index](::requirements::NodePtr node){
      auto parentNode = node->getParent();
      auto superParentNode = parentNode->getParent();
      if(superParentNode==nullptr) {
        return;
      }
      auto oldIndex = node->childIndex();
      auto newIndex = parentNode->childIndex()+1;
      auto parent = model.parent(index);
      auto superParent = model.parent(parent);
      model.beginMoveRows(parent, oldIndex, oldIndex, superParent, newIndex);
      node->levelUp();
      model.endMoveRows();
      emit change_to_viewpos(model.index(newIndex, 0, superParent));
      model.connector._batchthread.enqueue(std::make_unique<::requirements::commands::LevelUp>(node->getId()));
    });
  }

  void ModelManipulator::levelDown(const QModelIndex& index) {
    metaManipulator(index, [this, index](::requirements::NodePtr node){
      auto oldIndex = node->childIndex();
      if(oldIndex==0) {
        return;
      }
      auto parentNode = node->getParent();
      auto newParentNode = parentNode->getChildNr(oldIndex-1);
      auto newIndex = newParentNode->getChildren().size();
      auto superParent = model.parent(index);
      auto parent = model.index(oldIndex-1, 0, superParent);
      model.beginMoveRows(superParent, oldIndex, oldIndex, parent, newIndex);
      node->levelDown();
      model.endMoveRows();
      emit change_to_viewpos(model.index(newIndex, 0, parent));
      model.connector._batchthread.enqueue(std::make_unique<::requirements::commands::LevelDown>(node->getId()));
    });
  }

  void ModelManipulator::deleteNode(const QModelIndex& index) {
    metaManipulator(index, [this, index](::requirements::NodePtr node){

      auto parentIndex = model.parent(index);
      auto oldRow = node->childIndex();

      model.beginRemoveRows(parentIndex, oldRow, oldRow);
      auto trashId = model.model.nodeCollection->nodeToTrash(node);
      model.endRemoveRows();

      model.connector._batchthread.enqueue(std::make_unique<::requirements::commands::Trash>(node->getId(), trashId));
    });
  }

  void ModelManipulator::send_command(const std::string &command) {
    model.connector.send_command(command);
  }

  void ModelManipulator::newBlob(const std::string& filename){
    model.connector._batchthread.enqueue(std::make_unique<::requirements::commands::NewBlob>(filename));
  }

  ModelManipulator::ModelManipulator(Model &a_model)
    : model(a_model){}

}
