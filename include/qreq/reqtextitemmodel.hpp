#pragma once

#include "qreq/reqtextitemdata.hpp"

#include <QStandardItemModel>
#include <QObject>
#include <QVariant>

namespace qreq{

class ReqTextItemModel : public QStandardItemModel{
  ReqTextItemData _reqtextitemdata;
public:
  ReqTextItemModel(QObject *parent=Q_NULLPTR);
  ~ReqTextItemModel();
  QVariant data(const QModelIndex& index, int role=Qt::DisplayRole)const;
  bool setData(QModelIndex const& index, QVariant const& value, int role=Qt::EditRole);
};

}
