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

  static const int innerBorder = 5;
  static const int gap = 5;

  auto& rect = option.rect;
  auto left = rect.x();
  auto top = rect.y();
  auto width = rect.width();
  auto height = rect.height();
  auto right = rect.right();
  auto bottom = rect.bottom();

  QFontMetrics fm(option.font);
  int textHeight = fm.height();

  auto innerLeft = left+innerBorder;
  auto innerRight = right-innerBorder;
  auto innerTop = top+innerBorder;
  auto boxTop = innerTop+textHeight+2*gap;
  auto innerBottom = bottom-innerBorder;
  auto innerWidth = width-2*innerBorder;
  auto boxHeight = height-textHeight-4*gap;

  painter->save();
  painter->drawLine(QPoint(innerLeft, innerTop), QPoint(innerRight, innerTop));
  painter->drawLine(QPoint(innerLeft, innerTop), QPoint(innerLeft, boxTop));
  painter->drawLine(QPoint(innerRight, innerTop), QPoint(innerRight, boxTop));
  painter->drawLine(QPoint(innerLeft, boxTop), QPoint(innerRight, boxTop));
  painter->drawLine(QPoint(innerLeft, boxTop), QPoint(innerLeft, innerBottom));
  painter->drawLine(QPoint(innerRight, boxTop), QPoint(innerRight, innerBottom));
  painter->drawLine(QPoint(innerLeft, innerBottom), QPoint(innerRight, innerBottom));

  auto caption = ::requirements::id_to_string(node->getId());

  auto captionRect = QRect(innerLeft+gap, innerTop+gap, innerWidth-2*gap, textHeight);

  painter->setPen(QColor(0,0,255));
  painter->drawText(captionRect,  Qt::AlignTop, caption.c_str());

  auto boxRect = QRect(innerLeft+gap, boxTop+gap, innerWidth-2*gap, boxHeight);

  QTextDocument document(node->getContent().c_str());

  painter->translate(boxRect.left(), boxRect.top());
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
  return QSize(option.rect.width(), size.height()+50);
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

