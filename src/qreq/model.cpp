#include "qreq/model.hpp"

#include "requirements/commands/setcontent.hpp"

#include <iostream>

namespace qreq {

  void Model::updateContent(const QModelIndex& index, const std::string& content) {
    if(!index.isValid()) {
      return;
    }
    auto node = getNodeFromModelIndex(index);
    if(!node) {
      return;
    }
    connector._batchthread.enqueue(std::make_unique<::requirements::commands::SetContent>(node->getId(), content));
  }

  Qt::ItemFlags Model::flags(const QModelIndex& index) const {
    if(!index.isValid()) {
      return Qt::ItemIsEnabled;
    }
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
  }

  Model& Model::getModel(const QModelIndex& index) {
    auto* result=const_cast<Model*>(dynamic_cast<const Model*>(index.model()));
    assert(result!=nullptr);
    return *result;
  }

  ::requirements::NodePtr Model::getNodeFromModelIndex(const QModelIndex& index) const {
    if(!index.isValid()) {
      return nullptr;
    }
    return static_cast<::requirements::Node*>(index.internalPointer());
  }

  void Model::checkResponses() {
    ::requirements::batch::Response intermediate;
    if(connector.consumeResponse(intermediate)) {
      emit beginResetModel();
      model = std::move(intermediate);
      emit endResetModel();
    }
  }

  QModelIndex Model::index(int row, int column, const QModelIndex &parentModelIndex) const {
    if(column!=0) {
      return QModelIndex();
    }
    ::requirements::Node* parentNode;
    if(parentModelIndex.isValid()) {
      parentNode = static_cast<::requirements::Node *>(parentModelIndex.internalPointer());
    } else {
      if(!model.nodeCollection) {
        return QModelIndex();
      }
      parentNode = model.nodeCollection->getRootNode().get();
    }
    auto node = parentNode->getChildNr(row);
    if (!node) {
      return QModelIndex();
    }
    return createIndex(row, 0, static_cast<void *>(node.get()));
  }

  QModelIndex Model::parent(const QModelIndex &nodeModelIndex) const {
    if(!nodeModelIndex.isValid()) {
      return QModelIndex();
    }
    auto *node = static_cast<::requirements::Node *>(nodeModelIndex.internalPointer());
    auto parent = node->getParent();
    if (!parent) {
      return QModelIndex();
    }
    return createIndex(parent->childIndex(), 0, static_cast<void *>(parent.get()));
  }

  int Model::rootRowCount() const {
    if(!model.nodeCollection) {
      return 0;
    }
    return model.nodeCollection->getRootNode()->getChildren().size();
  }

  int Model::rowCount(const QModelIndex &nodeModelIndex) const {
    // Whats the exact meaning?
    if(!nodeModelIndex.isValid()) {
      std::cout<<"Root Row Count:"<<rootRowCount()<<std::endl;
      return rootRowCount();
    }
    auto node = static_cast<::requirements::Node *>(nodeModelIndex.internalPointer());
    return node->getChildren().size();
  }

  int Model::columnCount(const QModelIndex &) const {
    return 1;
  }

  QVariant Model::data(const QModelIndex &, int) const {
    return QVariant();
  }
}