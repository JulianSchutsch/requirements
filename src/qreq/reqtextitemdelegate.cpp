#include "qreq/reqtextitemdelegate.hpp"
#include "qreq/reqtextitemwidget.hpp"

#include <QKeyEvent>
#include <QTextEdit>

///Wir wollen mehrere Strings haben:
///caption und maintext
///Der dem Item zugeordnete QVariant enthält also eine Stringlist in der Reihenfolge
///caption,maintext

namespace qreq{

ReqTextItemDelegate::ReqTextItemDelegate(QObject *parent)
 : QItemDelegate(parent){
}

QWidget *ReqTextItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &) const{
  //QTextEdit *editor = new QTextEdit(parent);
  ReqTextItemWidget *editor=new ReqTextItemWidget(parent);

  return editor;
}

void ReqTextItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const{
  //QString value = index.model()->data(index, Qt::EditRole).toString();
  QStringList valuelist = index.model()->data(index, Qt::EditRole).toStringList();

  //QTextEdit *textedit=static_cast<QTextEdit*>(editor);
  ReqTextItemWidget *widget=static_cast<ReqTextItemWidget*>(editor);
  //textedit->setPlainText(value);
  if(valuelist.size()>1){
    widget->set_caption(valuelist[0].toStdString());
    widget->set_maintext(valuelist[1].toStdString());
  }
}

void ReqTextItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const{
  //QTextEdit *textedit=static_cast<QTextEdit*>(editor);
  //QString value = textedit->toPlainText();
  //model->setData(index, value, Qt::EditRole);
  ReqTextItemWidget *widget=static_cast<ReqTextItemWidget*>(editor);
  //QString value=widget->get_maintext().c_str();
  QStringList valuelist;
  valuelist << widget->get_caption().c_str();
  valuelist << widget->get_maintext().c_str();
  model->setData(index,valuelist,Qt::EditRole);
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
