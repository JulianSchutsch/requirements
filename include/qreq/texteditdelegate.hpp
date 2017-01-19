#pragma once

#include <QTextEdit>
#include <QItemDelegate>
#include <QWidget>
#include <QStandardItemModel>

namespace qreq{

class TextEditDelegate : public QItemDelegate{
  Q_OBJECT

public:
  TextEditDelegate(QObject *parent = 0);

  QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                       const QModelIndex &index) const;

  void setEditorData(QWidget *editor, const QModelIndex &index) const;
  void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

  void updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

}
