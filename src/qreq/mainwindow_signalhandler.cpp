#include "qreq/mainwindow.hpp"

#include <QApplication>

#include <iostream>

namespace qreq{

void MainWindow::on_f1button_clicked(){
}

void MainWindow::on_f2button_clicked(){
}

void MainWindow::on_f3button_clicked(){
}

void MainWindow::on_f4button_clicked(){
}

void MainWindow::on_f5button_clicked(){
}

void MainWindow::on_f7button_clicked(){
}

void MainWindow::on_f8button_clicked(){
}

void MainWindow::on_f9button_clicked(){
}

void MainWindow::on_f10button_clicked(){
  QApplication::exit(0);
}

void MainWindow::on_newblobbutton_clicked(){
}

void MainWindow::on_linkblobbutton_clicked(){
}

//bool MainWindow::eventFilter(QObject *object, QEvent *event) {
//  if (event->type() == QEvent::KeyPress) {
//    QKeyEvent* key_event = static_cast<QKeyEvent*>(event);
//    std::cout << std::hex << key_event->key() << std::dec << std::endl;
//    switch(key_event->key()){
//    case Qt::Key_F10:
//      on_f10button_clicked();
//      break;
//    default:
//      break;
//    }
//
//    //std::cout << "key" << key_event->key() << "object" << object << std::endl;
//  }
//  return false;
//}

void MainWindow::keyPressEvent(QKeyEvent *event){
  if(event->modifiers()&Qt::ShiftModifier){
    //std::cout << "shift + ";
    switch(event->key()){
    default:
      std::cout << "shift + " << std::hex << event->key() << std::dec << std::endl;
      break;
    }
  }
  else if(event->modifiers()&Qt::ControlModifier){
    switch(event->key()){
    default:
      std::cout << "ctrl + " << std::hex << event->key() << std::dec << std::endl;
      break;
    }
  }
  else if(event->modifiers()&Qt::AltModifier){
    switch(event->key()){
    default:
      std::cout << "alt + " << std::hex << event->key() << std::dec << std::endl;
      break;
    }
  }
  else{
    switch(event->key()){
    case Qt::Key_F1:
      on_f1button_clicked();
      break;
    case Qt::Key_F2:
      on_f2button_clicked();
      break;
    case Qt::Key_F3:
      on_f3button_clicked();
      break;
    case Qt::Key_F4:
      on_f4button_clicked();
      break;
    case Qt::Key_F5:
      on_f5button_clicked();
      break;
    case Qt::Key_F7:
      on_f7button_clicked();
      break;
    case Qt::Key_F8:
      on_f8button_clicked();
      break;
    case Qt::Key_F9:
      on_f9button_clicked();
      break;
    case Qt::Key_F10:
      on_f10button_clicked();
      break;
    default:
      break;
    }
  }
}

void MainWindow::on_openact_triggered(QString const& filename){
  //std::cout << "triggered: " << filename.toStdString() << std::endl;
  set_current_project(filename.toStdString());
}

void MainWindow::on_reqtree_expanded(const QModelIndex& i){
  _reqtree->resizeColumnToContents(i.column());
}

}
