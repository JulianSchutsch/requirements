#include "qreq/commandline.hpp"

#include <iostream>

namespace qreq{

CommandLine::CommandLine(QWidget *parent) : QLineEdit(parent){
  _commandstack.clear();
  _commandpointer=0;
}

CommandLine::~CommandLine(){
}

void CommandLine::keyPressEvent(QKeyEvent *event){

  if(event->modifiers()==Qt::NoModifier){
    switch(event->key()){
    case Qt::Key_Up:
      //change text to previous command on stack
      if(_commandpointer>0){
        //up is possible
        --_commandpointer;
        //change text
        if(_commandstack.size()>_commandpointer){
          setText(_commandstack[_commandpointer].c_str());
        }
      }
      break;
    case Qt::Key_Down:
      //change text to next command on stack
      if(_commandpointer<_commandstack.size()){
        //down is possible
        ++_commandpointer;
        //change text
        if(_commandstack.size()>_commandpointer){
          setText(_commandstack[_commandpointer].c_str());
        }
        else{
          setText("");
        }
      }

      break;
    case Qt::Key_Return:
      //Add text to commandstack
      if(!(text().isEmpty())){
        _commandstack.push_back(text().toStdString());
        _commandpointer=_commandstack.size();
        emit fire_command(text().toStdString());
        setText("");
      }
      break;
    default:
      QLineEdit::keyPressEvent(event);
      break;
    }
  }
  else
    QLineEdit::keyPressEvent(event);

}

}
