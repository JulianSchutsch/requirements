#include "qreq/mainwindow.hpp"
#include "qreq/keycodewindow.hpp"
#include "qreq/settings.hpp"
#include "qreq/blobselector.hpp"

#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QClipboard>

namespace qreq{

void MainWindow::on_f1button_clicked(){
  {
    //Standardbehandlung: Keycodes anzeigen
    KeyCodeWindow kcw(this);
    kcw.exec();
  }
}

void MainWindow::on_f2button_clicked(){
  {
    //Standardbehandlung: versetzen des aktuellen Knotens in den Edit-Mode
    _reqtree->edit(_reqtree->currentIndex());
  }
}

void MainWindow::on_f3button_clicked(){
  {
    //Standardbehandlung: Open Directory
    QString dirname=QFileDialog::getExistingDirectory(this,tr("Select project"));
    if(!(dirname.isEmpty())){
      Settings::getInstance().add_project(dirname.toStdString());
      set_current_project(dirname.toStdString());
      generate_menu_recent();
    }
  }
}

void MainWindow::on_f4button_clicked(){
  {
    //Standardbehandlung: erstmal nichts
  }
}

void MainWindow::on_f5button_clicked(){
  {
    //Create new node as sibling and copy content of old node into new node
    manipulator.newTwin(_reqtree->currentIndex());
  }
}

void MainWindow::on_f6button_clicked(){
  {
    //Standardbehandlung: New Sibling
    manipulator.newSibling(_reqtree->currentIndex());
  }
}

void MainWindow::on_f7button_clicked(){
  {
    //Standardbehandlung: New Child
    const auto index=_reqtree->currentIndex();
    manipulator.newChild(index);
    _reqtree->expand(index);
  }
}

void MainWindow::on_f8button_clicked(){
  {
    //Standardbehandlung: delete
    manipulator.deleteNode(_reqtree->currentIndex());
  }
}

void MainWindow::on_f9button_clicked(){
  {
    //Standardbehandlung: About
    QMessageBox::about(this,"QReq","Authors:\nJulian Schutsch\nDirk Neumann\nWebsite: https://github.com/JulianSchutsch/requirements");
  }
}

void MainWindow::on_f10button_clicked(){
  {
    //Standardbehandlung: Beenden
    QApplication::exit(0);
  }
}

void MainWindow::keyPressEvent(QKeyEvent *event){
  if((event->modifiers()==(Qt::AltModifier|Qt::ControlModifier))){
    switch(event->key()){
    default:
      std::cout << "ctrl+alt + " << std::hex << event->key() << std::dec << std::endl;
      break;
    }
  }
  else if(event->modifiers()&Qt::ShiftModifier){
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
    //Hier muss noch auf die Buttonmap gewechselt werden
    //
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
  auto node=model.getNodeFromModelIndex(i);
  if(node!=nullptr){
    QString commandtext=_commandline->text();
    commandtext+=::requirements::id_to_string(node->getId()).c_str();
    _commandline->setText(commandtext);
  }
  else{
    //Ach, was solls. Schau ich mal nach Schokolade.
  }
}

void MainWindow::on_reqtree_ctrl_alt_return(const QModelIndex &i){
  auto node=model.getNodeFromModelIndex(i);
  if(node!=nullptr){
    std::string idname=::requirements::id_to_string(node->getId()).c_str();
    //Jetzt kopieren in die Zwischenablage
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(idname.c_str());
  }
  else{
    //War keiner gewählt. Naja, dann nicht.
  }
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

void MainWindow::on_model_reset(){
  const auto oldindex=_reqtree->currentIndex();
  const auto sib=_reqtree->indexAt(_reqtree->rect().topLeft());
  if(model.rootRowCount()>0){
    _reqtree->setCurrentIndex(sib);
    _reqtree->expandAll();
    _reqtree->setCurrentIndex(oldindex);
  }
}

void MainWindow::on_manipulate_viewpos(const QModelIndex& index){
  _reqtree->scrollTo(index);
  _reqtree->setCurrentIndex(index);
}

}
