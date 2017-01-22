#include "qreq/mainwindow.hpp"
#include "qreq/keycodewindow.hpp"
#include "qreq/settings.hpp"

#include <QApplication>
#include <QFileDialog>

#include <iostream>

namespace qreq{

void MainWindow::on_f1button_clicked(){
  KeyCodeWindow kcw(this);
  kcw.exec();
}

void MainWindow::on_f2button_clicked(){
  store_collection();
}

void MainWindow::on_f3button_clicked(){
  //Open Directory
  QString dirname=QFileDialog::getExistingDirectory(this,tr("Select project"));
  if(!(dirname.isEmpty())){
    Settings::getInstance().add_project(dirname.toStdString());
    set_current_project(dirname.toStdString());
    generate_menu_recent();
  }
}

void MainWindow::on_f4button_clicked(){
  //versetzen des aktuellen Knotens in den Edit-Mode
  _reqtree->edit(_reqtree->currentIndex());
  //mal im QTreeView nachschauen und die methode aufrufen, die bei Doppelklick aufgerufen wird.
}

void MainWindow::on_f5button_clicked(){
  //Create new node and copy content of old node into new node
  new_node(true);
}

void MainWindow::on_f7button_clicked(){
  new_node(false);
}

void MainWindow::on_f8button_clicked(){
  //Delete the selected node
  QModelIndex index=_reqtree->currentIndex();
  std::string uuid=get_uuid_by_modelindex(index);
  if(uuid!=""){
    //Lösche den Knoten in der Collection
    requirements::NodePtr node=get_node_for_uuid(uuid);
    _currentStorage->getNodeCollection().deleteNode(node);
    //Lösche den Knoten im TreeModel
    QStandardItem *item_parent=get_parent_item_by_modelindex(index);
    if(item_parent!=nullptr){
      _reqmodel->removeRow(index.row(),index.parent());
    }
  }
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

void MainWindow::on_reqmodel_item_changed(QStandardItem* item){
  std::cout << "on_reqmodel_item_changed" << std::endl;
}

void MainWindow::on_reqtree_ctrl_left(const QModelIndex& i){
  std::cout << "on_reqtree_ctrl_left" << std::endl;
  std::cout << get_uuid_by_modelindex(i) << std::endl;
  std::cout << get_text_by_modelindex(i) << std::endl;

}

void MainWindow::on_reqtree_ctrl_right(const QModelIndex& i){
  std::cout << "on_reqtree_ctrl_right" << std::endl;
}

void MainWindow::on_reqtree_ctrl_up(const QModelIndex& i){
  std::cout << "on_reqtree_ctrl_up" << std::endl;
}

void MainWindow::on_reqtree_ctrl_down(const QModelIndex& i){
  std::cout << "on_reqtree_ctrl_down" << std::endl;
}

void MainWindow::on_reqtree_alt_return(const QModelIndex& i){
  //copy id to commandline
  std::string id=get_uuid_by_modelindex(i);
  QString commandtext=_commandline->text();
  commandtext+=id.c_str();
  _commandline->setText(commandtext);
}

void MainWindow::on_commandline_return(std::string const& command){
  std::cout << "command entered: " << command << std::endl;
}

}
