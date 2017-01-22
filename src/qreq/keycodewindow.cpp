#include "qreq/keycodewindow.hpp"

#include <QVBoxLayout>

namespace qreq{

  const char *keycodecontent =
  "F1: Keycodes\n"
  "F2: Save\n"
  "F4: Edit\n"
  "F5: Copy\n"
  "F7: New\n"
  "F8: Delete\n"
  "F9: About\n"
  "F10: Exit\n"
  "Ctrl+left: Move node up one level\n"
  "Ctrl+right: Move node down one level\n"
  "Ctrl+up: Move node up\n"
  "Ctrl+down: Move node down\n"
  "Alt+Return: Copy UUID to command line\n"
  "Ctrl+Return: Quit edit element\n"
  "ESC: Quit edit mode\n"
  "+: Expand node\n"
  "-: Collapse node\n";

KeyCodeWindow::KeyCodeWindow(QWidget *parent) : QDialog(parent){
  _textedit=new QTextEdit();
  _textedit->setReadOnly(true);
  QVBoxLayout *vbox=new QVBoxLayout();
  vbox->addWidget(_textedit);
  setLayout(vbox);
  fill_content();
}

KeyCodeWindow::~KeyCodeWindow(){
}

void KeyCodeWindow::fill_content(){
  QString plaintext(keycodecontent);
  _textedit->setPlainText(plaintext);
}


}
