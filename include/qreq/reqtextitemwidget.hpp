#pragma once

#include <QWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QLabel>

#include <string>

namespace qreq{

class ReqTextItemWidget : public QWidget{
  Q_OBJECT
  QLabel *_captionlabel;
  QTextEdit *_textedit;
  std::string _reqid;
public:
  ReqTextItemWidget(QWidget *parent=nullptr);
  ~ReqTextItemWidget();
  void set_caption(std::string const& caption);
  void set_maintext(std::string const& maintext);
  void set_reqid(std::string const& reqid) {_reqid=reqid;}
  std::string get_caption()const;
  std::string get_maintext()const;
  std::string get_reqid()const {return _reqid;}
};

}
