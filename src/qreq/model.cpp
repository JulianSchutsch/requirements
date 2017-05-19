#include "qreq/model.hpp"
#include "qreq/settings.hpp"

#include "requirements/commands/setcontent.hpp"

namespace qreq {

  void Model::setCurrentEditor(const QModelIndex& index, QTextEdit* editor) {
    currentEditor = editor;
    currentEdited = index.internalId();
  }
  void Model::clearCurrentEditor() {
    currentEditor = nullptr;
  }

  QModelIndex Model::editedIndex() {
    if(currentEditor==nullptr) {
      return QModelIndex();
    }
    auto nodeIt = lookup.find(currentEdited);
    if(nodeIt==lookup.end()) {
      return QModelIndex();
    }
    return createIndex(nodeIt->second->childIndex(), 0, currentEdited);
  }

  QString Model::editorContent() {
    if(currentEditor==nullptr) {
      return "";
    }
    return currentEditor->toPlainText();
  }

  bool Model::isEditing(const QModelIndex &index) {
    if(currentEditor==nullptr) {
      return false;
    }
    return index.internalId()==currentEdited;
  }

  void Model::updateContent(const QModelIndex& index, const std::string& content) {
    if(!index.isValid()) {
      return;
    }
    auto node = getNodeFromModelIndex(index);
    if(!node) {
      return;
    }
    node->updateContent(content);
    emit dataChanged(index, index);
    connector._batchthread.enqueue(std::make_unique<::requirements::commands::SetContent>(node->getId(), content));
  }

  Qt::ItemFlags Model::flags(const QModelIndex& index) const {
    if(!index.isValid()) {
      return Qt::ItemIsEnabled;
    }
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
  }

  Model& Model::getModel(const QModelIndex& index) {
    if(index.isValid()==true){
      auto* result=const_cast<Model*>(dynamic_cast<const Model*>(index.model()));
      assert(result!=nullptr);
      return *result;
    }
    else{
      Model* result=nullptr;
      return *result;
    }

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

  const std::string& Model::getFolder() {
    return model.status->folder;
  }

  ModelChange Model::checkResponses() {
    ::requirements::batch::Response intermediate;
    if(connector.consumeResponse(intermediate)) {
      if(!model.nodeCollection || *intermediate.nodeCollection!=*model.nodeCollection) {
        std::cout<<"Replace data"<<std::endl;
        beginResetModel();
        model = std::move(intermediate);
        lookup.clear();
        reverseLookup.clear();
        endResetModel();
      } else {
        std::cout<<"Keep core collection, replace the rest"<<std::endl;
        // Trick to move everything but the nodecollection, this keeps the model stable.
        auto keepCollection = std::move(model.nodeCollection);
        model = std::move(intermediate);
        model.nodeCollection = std::move(keepCollection);
        emit dataChanged(QModelIndex(), QModelIndex());
      }
      //So, damit kommts in die Liste der jüngsten Projekte
      if(model.status->folder!=currentFolder) {
        return ModelChange::Folder;
      }
    }
    return ModelChange::Common;
  }

  quintptr Model::insertLookup(const ::requirements::NodePtr& node) const {
    auto it = reverseLookup.find(node.get());
    if(it!=reverseLookup.end()) {
      return it->second;
    }
    auto index = lookupIndex++;
    lookup[index] = node.get();
    reverseLookup[node.get()] = index;
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
    assert(node->childIndex()==row);
    return createIndex(row, 0, insertLookup(node));
  }

  QModelIndex Model::parent(const QModelIndex &nodeModelIndex) const {
    auto node = getNodeFromModelIndex(nodeModelIndex);
    if(!node) {
      return QModelIndex();
    }
    auto parent = node->getParent();
    if (!parent || parent->getParent()==nullptr) {
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
