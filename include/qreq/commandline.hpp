#pragma once

#include <QLineEdit>
#include <QKeyEvent>

#include <vector>
#include <string>

namespace qreq{

  using strvec=std::vector<std::string>;

class CommandLine : public QLineEdit{
  Q_OBJECT
  strvec _commandstack{};
  strvec::size_type _commandpointer{};
protected:
  void keyPressEvent(QKeyEvent *event);
signals:
  void fire_command(std::string const& command);
public:
  CommandLine(QWidget *parent,strvec const& initial_commands);
  ~CommandLine();
  strvec getLastCommands(strvec::size_type count)const;
};

}
