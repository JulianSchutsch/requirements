#include "qreq/texteditdelegate.hpp"

namespace qreq{

TextEditDelegate::TextEditDelegate(QObject *parent)
 : QItemDelegate(parent){
}

QWidget *TextEditDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &) const{
   QTextEdit *editor = new QTextEdit(parent);
   //editor->addItem("A");
   //editor->addItem("B");
   //editor->addItem("C");
   //editor->addItem("D");

   return editor;
}

void TextEditDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const{
   QString value = index.model()->data(index, Qt::EditRole).toString();

   QTextEdit *textedit=static_cast<QTextEdit*>(editor);
   textedit->setPlainText(value);
   //QComboBox *cBox = static_cast<QComboBox*>(editor);
   //cBox->setCurrentIndex(cBox->findText(value));
}

void TextEditDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const{
  QTextEdit *textedit=static_cast<QTextEdit*>(editor);

  //QComboBox *cBox = static_cast<QComboBox*>(editor);

  QString value = textedit->toPlainText();

  model->setData(index, value, Qt::EditRole);
}

void TextEditDelegate::updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem &option, const QModelIndex &) const{
  editor->setGeometry(option.rect);
}

}
