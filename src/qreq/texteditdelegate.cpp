#include "qreq/texteditdelegate.hpp"

namespace qreq{

TextEditDelegate::TextEditDelegate(QObject *parent)
 : QItemDelegate(parent){
}

QWidget *TextEditDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &) const{
  QTextEdit *editor = new QTextEdit(parent);

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

}
//TODO KeyPressEvent überschreiben, Ctrl+Return rausfischen und dann closeEditor Oder doch irgendwie anders?
//An welcher Stelle wäre es denn richtig? QTreeView? QStandardItemModel? QStandardItem? QItemDelegate?
