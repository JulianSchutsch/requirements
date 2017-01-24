#pragma once

#include <QWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QLabel>


namespace qreq{

class ReqTextItemWidget : public QWidget{
  Q_OBJECT
  QLabel *_captionlabel;
  QTextEdit *_textedit;
public:
  ReqTextItemWidget(QWidget *parent);
  ~ReqTextItemWidget();
  void set_caption(std::string const& caption);
  void set_maintext(std::string const& maintext);
  std::string const& get_caption()const;
  std::string const& get_maintext()const;

};

}
