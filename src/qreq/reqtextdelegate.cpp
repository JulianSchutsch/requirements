#include "qreq/reqtextdelegate.hpp"
#include "qreq/reqtextitemwidget.hpp"
#include "qreq/model.hpp"

#include <QKeyEvent>
#include <QPainter>

#include <iostream>
#include <include/requirements/batch/response.hpp>

//TODO: Signal-Handling verbessern: F4 aus dem Hauptfenster muss direkt im TextEdit landen.
//Dort drin muss ctrl+Return abgefangen werden. Jetzt wird das noch auf der Ebene des ReqTextItemWidget gemacht

namespace qreq{

ReqTextDelegate::ReqTextDelegate(QObject *parent) {}

void ReqTextDelegate::paint(QPainter *painter,const QStyleOptionViewItem &option, QModelIndex const& index) const{

  if (option.state & QStyle::State_Selected) painter->fillRect(option.rect, option.palette.highlight());

  auto& model = Model::getModel(index);
  auto node = model.getNodeFromModelIndex(index);
  if(!node) {
    return;
  }

  painter->save();
  painter->translate(QPointF(option.rect.x(), option.rect.y()));
  QTextDocument document(node->getContent().c_str());
  document.drawContents(painter);
  painter->restore();

}

QSize ReqTextDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const{
  auto& model = Model::getModel(index);
  auto node = model.getNodeFromModelIndex(index);
  if(!node) {
    return QSize(200,200);
  }
  QTextDocument document(node->getContent().c_str());
  document.setTextWidth(option.rect.width());
  auto size = document.size();
  return QSize(size.width()+20, size.height()+20);
}

QWidget *ReqTextDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &) const{
  std::cout<<"Start editing"<<std::endl;
  //ReqTextItemWidget *editor=new ReqTextItemWidget(parent);
  return nullptr;
}

void ReqTextDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const{
  ReqTextItemWidget *item_widget=static_cast<ReqTextItemWidget*>(editor);
  //item_widget->setModelIndex(index);
}

void ReqTextDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const{
  ReqTextItemWidget *widget=static_cast<ReqTextItemWidget*>(editor);
}

void ReqTextDelegate::updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem &option, const QModelIndex &) const{
  editor->setGeometry(option.rect);
}

bool ReqTextDelegate::eventFilter(QObject *editor, QEvent *event){
  if (event->type() == QEvent::KeyPress) {
    QKeyEvent* key_event = static_cast<QKeyEvent*>(event);
    if(key_event->modifiers()==Qt::ControlModifier){
      if(key_event->key()==Qt::Key_Return){
        emit commitData(static_cast<QWidget*>(editor));
        emit closeEditor(static_cast<QWidget*>(editor),QAbstractItemDelegate::EditNextItem);
        return true;
      }
    }
  }
}

}

