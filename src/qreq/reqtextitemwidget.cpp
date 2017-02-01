#include "qreq/reqtextitemwidget.hpp"

#include <QString>
#include <QVBoxLayout>
#include <QEvent>

#include "qreq/model.hpp"

#include <iostream>

namespace qreq{

  //TODO: Rahmen drum malen
ReqTextItemWidget::ReqTextItemWidget(QWidget *parent) : QWidget(parent){
  _captionlabel=new QLabel();
  _textedit=new QTextEdit();
  _textedit->setMinimumHeight(20);
  setMinimumHeight(20);

  //Hier wird der Focus, wenn er kommt, ans textedit übertragen
  setFocusProxy(_textedit);
  QVBoxLayout *vbox=new QVBoxLayout();
  //vbox->addWidget(_captionlabel);  //TODO: zum Testen mal raus
  vbox->addWidget(_textedit);
  setLayout(vbox);
}

void ReqTextItemWidget::saveToModel(QAbstractItemModel *model, const QModelIndex &index) {
  ModelItem mItem(index);
  if(!mItem.valid()) {
    return;
  }
  mItem.setContentFromQString(_textedit->toPlainText());
  mItem.saveToModel(model, index);
}

  void ReqTextItemWidget::setModelIndex(QModelIndex const& index) {
  ModelItem mItem(index);
  if(!mItem.valid()) {
    std::cout<<"Invalid model?"<<std::endl;
    // Ups, thats not a valid item..may this deserves a better response than silence (TODO)
    return;
  }
  _textedit->setPlainText(mItem.getContentAsQString());
  QSize oldsize=_textedit->size();
  oldsize.setHeight(0);
  _textedit->resize(oldsize);

}

}
