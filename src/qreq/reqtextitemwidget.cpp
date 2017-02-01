#include "qreq/reqtextitemwidget.hpp"

#include <QString>
#include <QVBoxLayout>
#include <QEvent>

#include "qreq/model.hpp"

#include <iostream>

namespace qreq{

  //TODO: Rahmen drum malen
  ReqTextItemWidget::ReqTextItemWidget(QModelIndex const& index, QWidget *parent) : QWidget(parent) {
    _captionlabel = new QLabel();
    _textedit = new QTextEdit();
    _textedit->setMinimumHeight(20);
    setMinimumHeight(20);

    //Hier wird der Focus, wenn er kommt, ans textedit übertragen
    setFocusProxy(_textedit);
    QVBoxLayout *vbox = new QVBoxLayout();
    //vbox->addWidget(_captionlabel);  //TODO: zum Testen mal raus
    vbox->addWidget(_textedit);
    setLayout(vbox);
    auto &model = Model::getModel(index);
    auto node = model.getNodeFromModelIndex(index);
    _textedit->setPlainText(node->getContent().c_str());
  }

}
