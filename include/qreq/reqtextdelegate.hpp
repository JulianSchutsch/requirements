#pragma once

#include <QTextEdit>
#include <QItemDelegate>
#include <QAbstractItemDelegate>
#include <QWidget>

namespace qreq{

  class Model;

  class ReqTextDelegate : public QAbstractItemDelegate{
    Q_OBJECT
  private:
    mutable Model* currentModel = nullptr;
  protected:
    bool eventFilter(QObject *editor, QEvent *event) override;
  public:
    ReqTextDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter,const QStyleOptionViewItem &option, QModelIndex const& index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                         const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem &option, const QModelIndex &index) const;
  private slots:
    void editorDestroyed(QObject *obj);
  };

}
