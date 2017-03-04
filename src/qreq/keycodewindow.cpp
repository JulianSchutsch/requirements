#include "qreq/keycodewindow.hpp"

#include <QVBoxLayout>

namespace qreq{

  const char *keycodecontent =
  "F1: Keycodes\n"
  "F2: Edit\n"
  "F3: Open Folder\n"
  "F4: No function\n"
  "F5: Copy\n"
  "F6: New Sibling\n"
  "F7: New Child\n"
  "F8: Delete\n"
  "F9: About\n"
  "F10: Exit\n"
  "Ctrl+left: Move node up one level\n"
  "Ctrl+right: Move node down one level\n"
  "Ctrl+up: Move node up\n"
  "Ctrl+down: Move node down\n"
  "Alt+Return: Copy UUID to command line\n"
  "Ctrl+Alt+Return: Copy UUID to clipboard\n"
  "Ctrl+Return: Quit edit element\n"
  "Ctrl+Alt+e: Expand all\n"
  "Ctrl+Alt+c: Collapse all\n"
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
