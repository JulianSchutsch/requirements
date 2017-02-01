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
    auto it = lookup.find(index.internalId());
    if(it==lookup.end()) {
      return nullptr;
    }
    return it->second;
  }

  void Model::checkResponses() {
    ::requirements::batch::Response intermediate;
    if(connector.consumeResponse(intermediate)) {
      model = std::move(intermediate);
      lookup.clear();
      emit layoutChanged();
    }
  }

  qint64 Model::insertLookup(const ::requirements::NodePtr& node) const {
    auto index = lookupIndex++;
    lookup[index] = node.get();
    return index;
  }

  QModelIndex Model::index(int row, int column, const QModelIndex &parentModelIndex) const {
    if(column!=0) {
      return QModelIndex();
    }
    auto parentNode = getNodeFromModelIndex(parentModelIndex);
    if(!parentNode) {
      if(!model.nodeCollection) {
        return QModelIndex();
      }
      parentNode = model.nodeCollection->getRootNode();
    }
    auto node = parentNode->getChildNr(row);
    if (!node) {
      return QModelIndex();
    }
    return createIndex(row, 0, insertLookup(node));
  }

  QModelIndex Model::parent(const QModelIndex &nodeModelIndex) const {
    auto node = getNodeFromModelIndex(nodeModelIndex);
    if(!node) {
      return QModelIndex();
    }
    auto parent = node->getParent();
    if (!parent) {
      return QModelIndex();
    }
    return createIndex(parent->childIndex(), 0, insertLookup(parent));
  }

  int Model::rootRowCount() const {
    if(!model.nodeCollection) {
      return 0;
    }
    return model.nodeCollection->getRootNode()->getChildren().size();
  }

  int Model::rowCount(const QModelIndex &nodeModelIndex) const {
    auto node = getNodeFromModelIndex(nodeModelIndex);
    if(!node) {
      return rootRowCount();
    }
    return node->getChildren().size();
  }

  int Model::columnCount(const QModelIndex &) const {
    return 1;
  }

  QVariant Model::data(const QModelIndex &, int) const {
    return QVariant();
  }
}