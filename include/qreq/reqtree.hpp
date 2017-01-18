#pragma once

#include <QTreeView>
#include <QKeyEvent>

namespace qreq{

class ReqTree : public QTreeView{
  Q_OBJECT
protected:
  void keyPressEvent(QKeyEvent *event);
signals:
  void ctrl_left_pressed(const QModelIndex& index);
  void ctrl_right_pressed(const QModelIndex& index);
  void ctrl_up_pressed(const QModelIndex& index);
  void ctrl_down_pressed(const QModelIndex& index);
  void alt_return_pressed(const QModelIndex& index);
public:
  ReqTree(QWidget *parent);
  ~ReqTree();
};

}
