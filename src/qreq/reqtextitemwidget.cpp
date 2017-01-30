#include "qreq/reqtextitemwidget.hpp"

#include <QString>
#include <QVBoxLayout>
#include <QEvent>

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

ReqTextItemWidget::~ReqTextItemWidget(){
}

void ReqTextItemWidget::set_caption(std::string const& caption){
  _captionlabel->setText(caption.c_str());
}

void ReqTextItemWidget::set_maintext(std::string const& maintext){
  _textedit->setPlainText(maintext.c_str());
  QSize oldsize=_textedit->size();
  oldsize.setHeight(0);
  _textedit->resize(oldsize);
}

std::string ReqTextItemWidget::get_caption()const{
  std::string retval=_captionlabel->text().toStdString();
  return retval;
}

std::string ReqTextItemWidget::get_maintext()const{
  std::string retval = _textedit->toPlainText().toStdString();

  return retval;
}

}
