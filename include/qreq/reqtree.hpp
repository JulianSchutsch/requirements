#pragma once

#include <QTreeView>
#include <QKeyEvent>

namespace qreq{

class ReqTree : public QTreeView{
  Q_OBJECT
protected:
  void keyPressEvent(QKeyEvent *event) override;

  int sizeHintForColumn(int column) const override {
    int viewWidth = viewport()->width();
    int colWidth = QTreeView::sizeHintForColumn(column);
    return std::max(viewWidth, colWidth);
  }
signals:
  void ctrl_left_pressed(const QModelIndex& index);
  void ctrl_right_pressed(const QModelIndex& index);
  void ctrl_up_pressed(const QModelIndex& index);
  void ctrl_down_pressed(const QModelIndex& index);
  void alt_return_pressed(const QModelIndex& index);
  void ctrl_alt_return_pressed(const QModelIndex& index);
public:
  ReqTree(QWidget *parent);
  ~ReqTree();
};

}
