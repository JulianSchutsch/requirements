#pragma once

#include <QWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QLabel>
#include <QSize>
#include <QPoint>

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
  //Jetzt noch die Größe auf das neue Widget anpassen, d.h. die Höhe des oberen Labels berücksichtigen
  //Vielleicht kann man das noch automatischer berechnen, d.h. die Label-Höhe vom Widget bekommen...
  //Oder überhaupt die Größen aller Subwidget...
  static QSize size_hint_for_delegate_modifier(){return QSize(-20,60);}  //TODO diese Zahlen müssen automatisch bestimmt werden
  static QPoint region_for_delegate_modifier(){return QPoint(-94,-27);}  //TODO diese Zahlen müssen automatisch bestimmt werden
};

}
