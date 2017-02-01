#pragma once

#include <mutex>

#include <QString>
#include <QStandardItem>

#include "requirements/id.hpp"
#include "requirements/batch/response.hpp"

#include "qreq/threadconnector.hpp"

namespace qreq {

  class Model : public QAbstractItemModel {
  private:
    ThreadConnector connector;
    ::requirements::batch::Response model;
    int rootRowCount() const;
    friend class ModelManipulator;

    mutable quintptr lookupIndex;
    mutable std::map<quintptr, Node*> lookup;
    mutable std::map<Node*, quintptr> reverseLookup;

    quintptr insertLookup(const ::requirements::NodePtr& node) const;

  public:
    // This function is supposed to the connection between the batch thread and the main thread. It must be called in regular intervals to collect changes in the model.
    void checkResponses();

    static Model& getModel(const QModelIndex& index);
    ::requirements::NodePtr getNodeFromModelIndex(const QModelIndex& index) const;

    QModelIndex index(int row, int column, const QModelIndex& parentModelIndex) const override;
    QModelIndex parent(const QModelIndex& nodeModelIndex) const override;
    int rowCount(const QModelIndex& nodeModelIndex) const override;
    int columnCount(const QModelIndex&) const override;
    QVariant data(const QModelIndex&, int) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const;

    void updateContent(const QModelIndex& index, const std::string& content);
  };

}