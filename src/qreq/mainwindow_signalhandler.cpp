#include "qreq/mainwindow.hpp"
#include "qreq/keycodewindow.hpp"
#include "qreq/settings.hpp"
#include "qreq/blobselector.hpp"

#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>

#include <iostream>

namespace qreq{

void MainWindow::on_f1button_clicked(){
  KeyCodeWindow kcw(this);
  kcw.exec();
}

void MainWindow::on_f2button_clicked(){
  // No actual function left
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
}

void MainWindow::on_f5button_clicked(){
  //Create new node as sibling and copy content of old node into new node
}

void MainWindow::on_f6button_clicked(){
  manipulator.newSibling(_reqtree->currentIndex());
}

void MainWindow::on_f7button_clicked(){
  manipulator.newChild(_reqtree->currentIndex());
}

void MainWindow::on_f8button_clicked(){
  manipulator.deleteNode(_reqtree->currentIndex());
}

void MainWindow::on_f9button_clicked(){
  QMessageBox::about(this,"QReq","Authors:\nJulian Schutsch\nDirk Neumann\nWebsite: https://github.com/JulianSchutsch/requirements");
}

void MainWindow::on_f10button_clicked(){
  QApplication::exit(0);
}

//Adds a new blob to storage.
void MainWindow::on_newblobbutton_clicked(){
    // TODO: Seems an abort case for "nothing selected" is missing
/*  QString filename=QFileDialog::getOpenFileName(this,tr("Select file to be a blob"));
  std::string trblob=newblob(filename.toStdString());
  _threadconnector._batchthread.enqueue(std::make_unique<::requirements::commands::NewBlob>(filename.toStdString()));*/
}

void MainWindow::on_linkblobbutton_clicked(){
}

void MainWindow::keyPressEvent(QKeyEvent *event){
  if(event->modifiers()&Qt::ShiftModifier){
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
    case Qt::Key_F6:
      on_f6button_clicked();
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

void MainWindow::on_reqtree_ctrl_left(const QModelIndex& i){
    manipulator.levelUp(i);
}

void MainWindow::on_reqtree_ctrl_right(const QModelIndex& i){
    manipulator.levelDown(i);
}

//Move node up.
void MainWindow::on_reqtree_ctrl_up(const QModelIndex& i){
    manipulator.up(i);
}

//Move node down
void MainWindow::on_reqtree_ctrl_down(const QModelIndex& i){
    manipulator.down(i);
}

void MainWindow::on_reqtree_alt_return(const QModelIndex& i){
  QString commandtext=_commandline->text();
  _commandline->setText(commandtext);
}

void MainWindow::on_commandline_return(std::string const& command){
  std::cout << "command entered: " << command << std::endl;
  try {
    manipulator.send_command(command);
  } catch(::requirements::Exception& e) {
    BatchMessage m;
    m.message = e.getReason();
    m.parameters = e.getParameters();
    switch(e.getKind()) {
      case ::requirements::Exception::Kind::Internal:
        m.kind = ::requirements::Status::MessageKind::InternalError;
        break;
      case ::requirements::Exception::Kind::User:
        m.kind = ::requirements::Status::MessageKind::UserError;
        break;
      case ::requirements::Exception::Kind::Other:
        m.kind = ::requirements::Status::MessageKind::OtherError;
        break;
    }
    printMessage(m);
  }
}

}
