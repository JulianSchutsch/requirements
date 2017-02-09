#include "qreq/commandline.hpp"

#include <iostream>

namespace qreq{

CommandLine::CommandLine(QWidget *parent,strvec const& initial_commands) : QLineEdit(parent){
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

strvec CommandLine::getLastCommands(strvec::size_type count)const{
  if(count < _commandstack.size()){
    //Nur die letzten count Elements
    strvec retval;
    for(auto i=_commandstack.size()-count;i<_commandstack.size();++i){
      retval.push_back(_commandstack[i]);
    }
    return retval;
    //strvec::iterator it;
    //std::vector<std::string>::iterator it;
    //it=_commandstack.begin();
    //strvec retval;
    //retval.assign(it,_commandstack.end());
  }
  else{
    return _commandstack;
  }
}

}
