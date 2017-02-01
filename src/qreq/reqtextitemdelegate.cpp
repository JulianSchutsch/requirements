#include "qreq/reqtextitemdelegate.hpp"
#include "qreq/reqtextitemwidget.hpp"

#include <QKeyEvent>
#include <QTextEdit>

#include "qreq/model.hpp"

///Wir wollen mehrere Strings haben:
///caption und maintext
///Der dem Item zugeordnete QVariant enthält also eine Stringlist in der Reihenfolge
///caption,maintext

namespace qreq{

ReqTextItemDelegate::ReqTextItemDelegate(QObject *parent)
 : QItemDelegate(parent){
}

QWidget *ReqTextItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex & index) const{
  //QTextEdit *editor = new QTextEdit(parent);
  ReqTextItemWidget *editor=new ReqTextItemWidget(parent);

  return editor;
}

void ReqTextItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const{
  ReqTextItemWidget *widget=static_cast<ReqTextItemWidget*>(editor);
  widget->setModelIndex(index);
}

void ReqTextItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const{
  ReqTextItemWidget *widget=static_cast<ReqTextItemWidget*>(editor);
  // This should issue an edit command TODO
  widget->saveToModel(model, index);
}

void ReqTextItemDelegate::updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem &option, const QModelIndex &) const{
  editor->setGeometry(option.rect);
}

bool ReqTextItemDelegate::eventFilter(QObject *editor, QEvent *event){
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

  return QItemDelegate::eventFilter(editor,event);
}

}
