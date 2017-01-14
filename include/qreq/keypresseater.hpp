#pragma once

#include <QObject>
#include <QKeyEvent>

class KeyPressEater : public QObject
{
  Q_OBJECT

protected:
  bool eventFilter(QObject *obj, QEvent *event);
public:
  KeyPressEater(){}
  ~KeyPressEater(){}
};
