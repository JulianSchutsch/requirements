#pragma once

#include <QTextEdit>
#include <QItemDelegate>
//#include <QAbstractItemDelegate>
#include <QWidget>
#include <QStandardItemModel>
#include <QStyledItemDelegate>

namespace qreq{

class ReqTextDelegate : public QStyledItemDelegate{
  Q_OBJECT
protected:
  bool eventFilter(QObject *editor, QEvent *event);
public:
  ReqTextDelegate(QObject *parent = nullptr);

  void paint(QPainter *painter,const QStyleOptionViewItem &option, QModelIndex const& index) const override;
  QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
  QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                       const QModelIndex &index) const;

  void setEditorData(QWidget *editor, const QModelIndex &index) const;
  void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

  void updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

}
