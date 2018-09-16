#pragma once

#include <QDialog>
#include <QTextEdit>

namespace qreq{

class KeyCodeWindow : public QDialog{
  Q_OBJECT
  QTextEdit *_textedit=nullptr;
  void fill_content();
public:
  KeyCodeWindow(QWidget *parent);
  ~KeyCodeWindow();
  KeyCodeWindow(const KeyCodeWindow&) = delete;
  KeyCodeWindow& operator = (const KeyCodeWindow&) = delete;
};

}
