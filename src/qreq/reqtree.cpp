#include "qreq/reqtree.hpp"
#include "qreq/texteditdelegate.hpp"

#include <iostream>

namespace qreq{

ReqTree::ReqTree(QWidget *parent) : QTreeView(parent)
{
}

ReqTree::~ReqTree(){
}

//Wir wollen die Tasten Ctrl+Richtungstasten einfangen, und jeweils ein spezielles Signal
//senden, die anderen Tasten sollen normal verwendet werden
void ReqTree::keyPressEvent(QKeyEvent *event){
  if(event->modifiers()==Qt::ControlModifier){
    switch(event->key()){
    case Qt::Key_Left:
      std::cout << "Ctrl+Left" << std::endl;
      emit ctrl_left_pressed(currentIndex());
      break;
    case Qt::Key_Right:
      std::cout << "Ctrl+Right" << std::endl;
      emit ctrl_right_pressed(currentIndex());
      break;
    case Qt::Key_Up:
      std::cout << "Ctrl+Up" << std::endl;
      emit ctrl_up_pressed(currentIndex());
      break;
    case Qt::Key_Down:
      std::cout << "Ctrl+Down" << std::endl;
      emit ctrl_down_pressed(currentIndex());
      break;
    default:
      QTreeView::keyPressEvent(event);
      break;
    }
  }
  else if(event->modifiers()==Qt::AltModifier){
    switch(event->key()){
    case Qt::Key_Return:
      emit alt_return_pressed(currentIndex());
      break;
    default:
      QTreeView::keyPressEvent(event);
      break;
    }
  }
  //if(event->key() == Qt::Key_A && e->modifiers() == Qt::ControlModifier){
  //  e->ignore();
  //  QWidget::keyPressEvent(e); // Not sure about this. Please try and report!
  //}
  else
    QTreeView::keyPressEvent(event);
}

}
