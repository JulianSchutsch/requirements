#pragma once

#include <QString>
#include <QStandardItem>

#include "requirements/id.hpp"
#include "requirements/batch/response.hpp"

#include <iostream>

namespace qreq {

  class Model : public QAbstractItemModel {
  private:
    ::requirements::batch::Response model;
    int rootRowCount() const;
  public:
    static Model& getModel(const QModelIndex& index);
    ::requirements::NodePtr getNodeFromModelIndex(const QModelIndex& index) const;
    void consumeModel(::requirements::batch::Response&& a_model);

    QModelIndex index(int row, int column, const QModelIndex& parentModelIndex) const override;
    QModelIndex parent(const QModelIndex& nodeModelIndex) const override;
    int rowCount(const QModelIndex& nodeModelIndex) const override;
    int columnCount(const QModelIndex&) const override;
    QVariant data(const QModelIndex&, int) const override;
  };

}