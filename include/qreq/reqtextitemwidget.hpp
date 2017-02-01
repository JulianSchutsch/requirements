#pragma once

#include <QWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QLabel>
#include <QSize>
#include <QPoint>
#include <QAbstractItemModel>

#include <string>

namespace qreq{

class ReqTextItemWidget : public QWidget{
  Q_OBJECT
  QLabel *_captionlabel;
  QTextEdit *_textedit;
public:
  ReqTextItemWidget(QWidget *parent=nullptr);

  void setModelIndex(QModelIndex const& index);
  void saveToModel(QAbstractItemModel *model, const QModelIndex &index);

  //Jetzt noch die Größe auf das neue Widget anpassen, d.h. die Höhe des oberen Labels berücksichtigen
  //Vielleicht kann man das noch automatischer berechnen, d.h. die Label-Höhe vom Widget bekommen...
  //Oder überhaupt die Größen aller Subwidget...
  static QSize size_hint_for_delegate_modifier(){return QSize(-20,60);}  //TODO diese Zahlen müssen automatisch bestimmt werden
  static QPoint region_for_delegate_modifier(){return QPoint(-94,-27);}  //TODO diese Zahlen müssen automatisch bestimmt werden
};

}
