#pragma once

#include <QItemDelegate>
#include <QWidget>
#include <QStandardItemModel>

namespace qreq{

class ReqTextItemDelegate : public QItemDelegate{
  Q_OBJECT
protected:
  bool eventFilter(QObject *editor, QEvent *event);
public:
  ReqTextItemDelegate(QObject *parent = 0);

  QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                       const QModelIndex &index) const;

  void setEditorData(QWidget *editor, const QModelIndex &index) const;
  void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

  void updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

}
//In die Doku zu QAbstractItemDeelegate schauen bei der Progressbar
