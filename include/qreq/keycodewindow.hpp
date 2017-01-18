#pragma once

#include <QDialog>
#include <QTextEdit>

namespace qreq{

class KeyCodeWindow : public QDialog{
  Q_OBJECT
  QTextEdit *_textedit;
  void fill_content();
public:
  KeyCodeWindow(QWidget *parent);
  ~KeyCodeWindow();
};

}
