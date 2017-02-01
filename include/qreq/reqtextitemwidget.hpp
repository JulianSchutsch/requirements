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
  ReqTextItemWidget(const QModelIndex& index, QWidget *parent=nullptr);
};

}
