#pragma once

#include <QLineEdit>
#include <QKeyEvent>

#include <vector>
#include <string>

namespace qreq{

class CommandLine : public QLineEdit{
  Q_OBJECT
  std::vector<std::string> _commandstack;
  std::vector<std::string>::size_type _commandpointer;
protected:
  void keyPressEvent(QKeyEvent *event);
signals:
  void fire_command(std::string const& command);
public:
  CommandLine(QWidget *parent);
  ~CommandLine();

};

}
