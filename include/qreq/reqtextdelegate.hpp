#pragma once

#include <QTextEdit>
#include <QItemDelegate>
#include <QAbstractItemDelegate>
#include <QWidget>
#include <QTreeView>

#include "requirements/batch/response.hpp"

namespace qreq{

  class Model;

  class ReqTextDelegate : public QAbstractItemDelegate{
    Q_OBJECT
  private:
    class CellGeometry;

    mutable Model* currentModel = nullptr;

    QTreeView* treeView;

    void paintFrame(QPainter* painer, CellGeometry& geometry) const;
    void paintRequirementAcceptance(QPainter* painter, CellGeometry& geometry, const ::requirements::batch::Response& model, ::requirements::Id nodeId) const;
    void paintNodeDescription(QPainter* painter, CellGeometry& geometry, const ::requirements::batch::Response& model, ::requirements::Id nodeId) const;
    void paintAcceptanceAccepts(QPainter* painter, CellGeometry& geometry, const ::requirements::batch::Response& model, ::requirements::Id nodeId) const;

    void makeRegionVisible(QRect r, int margin);
    void makeCursorVisible();

  protected:
    bool eventFilter(QObject *editor, QEvent *event) override;
  public:
    ReqTextDelegate(QTreeView* treeview, QObject *parent = nullptr);

    void paint(QPainter *painter,const QStyleOptionViewItem &option, QModelIndex const& index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem &option, const QModelIndex &index) const;
  private slots:
    void editorDestroyed(QObject *obj);
    void cursorPositionChanged();
    void textChanged();
    void horizontalSliderRangeChanged(int min, int max);
    void verticalSliderRangeChanged(int min, int max);
  };

}
