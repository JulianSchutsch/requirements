#include "qreq/texteditdelegate.hpp"

#include <QKeyEvent>

#include <iostream>

namespace qreq{

TextEditDelegate::TextEditDelegate(QObject *parent)
 : QItemDelegate(parent){
}

QWidget *TextEditDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &) const{
  QTextEdit *editor = new QTextEdit(parent);
  //_editor = new QTextEdit(parent);

  return editor;
}

void TextEditDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const{
  QString value = index.model()->data(index, Qt::EditRole).toString();

  QTextEdit *textedit=static_cast<QTextEdit*>(editor);
  textedit->setPlainText(value);
}

void TextEditDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const{
  QTextEdit *textedit=static_cast<QTextEdit*>(editor);

  QString value = textedit->toPlainText();

  model->setData(index, value, Qt::EditRole);
}

void TextEditDelegate::updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem &option, const QModelIndex &) const{
  editor->setGeometry(option.rect);
}

bool TextEditDelegate::eventFilter(QObject *editor, QEvent *event){
  if (event->type() == QEvent::KeyPress) {
    QKeyEvent* key_event = static_cast<QKeyEvent*>(event);
    if(key_event->modifiers()==Qt::ControlModifier){
      if(key_event->key()==Qt::Key_Return){
        std::cout << "eventFilter delegate" << std::endl;
        emit commitData(static_cast<QWidget*>(editor));
        emit closeEditor(static_cast<QWidget*>(editor),QAbstractItemDelegate::EditNextItem);
        return true;
      }
    }
  }

  return QItemDelegate::eventFilter(editor,event);
}

}
