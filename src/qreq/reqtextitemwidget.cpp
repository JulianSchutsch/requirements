#include "qreq/reqtextitemwidget.hpp"

#include <QString>
#include <QVBoxLayout>

namespace qreq{

  //TODO: Rahmen drum malen
ReqTextItemWidget::ReqTextItemWidget(QWidget *parent) : QWidget(parent){
  _captionlabel=new QLabel();
  _textedit=new QTextEdit();
  QVBoxLayout *vbox=new QVBoxLayout();
  vbox->addWidget(_captionlabel);
  vbox->addWidget(_textedit);
  setLayout(vbox);
}

ReqTextItemWidget::~ReqTextItemWidget(){
}

void ReqTextItemWidget::set_caption(std::string const& caption){
  _captionlabel->setText(caption.c_str());
  //_captionlabel->setText("Rosenheiz");
}

void ReqTextItemWidget::set_maintext(std::string const& maintext){
  _textedit->setPlainText(maintext.c_str());
}

std::string ReqTextItemWidget::get_caption()const{
  std::string retval=_captionlabel->text().toStdString();
  return retval;
}

std::string ReqTextItemWidget::get_maintext()const{
  std::string retval = _textedit->toPlainText().toStdString();

  return retval;
}

//void ReqTextItemWidget::paintEvent(QPaintEvent *)
//{
//    QPainter painter(this);
//    _textedit->paint(&painter, rect(), this->palette(),
//                       StarRating::Editable);
//
//}

}
