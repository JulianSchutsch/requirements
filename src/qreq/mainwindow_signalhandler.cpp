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
}

void MainWindow::on_f5button_clicked(){
  //Create new node as sibling and copy content of old node into new node
  new_node(true,true);
}

void MainWindow::on_f6button_clicked(){
  //Create new node as sibling without content
  new_node(true,false);
}

void MainWindow::on_f7button_clicked(){
  //Create new node as child
  new_node(false,false);
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
  QMessageBox::about(this,"QReq","Authors:\nJulian Schutsch\nDirk Neumann\nWebsite: https://github.com/JulianSchutsch/requirements");
}

void MainWindow::on_f10button_clicked(){
  QApplication::exit(0);
}

//Adds a new blob to storage.
void MainWindow::on_newblobbutton_clicked(){
  if(_currentStorage){
    QString filename=QFileDialog::getOpenFileName(this,tr("Select file to be a blob"));
    std::string trblob=newblob(filename.toStdString());
    add_blob_to_row(_reqtree->currentIndex(),trblob);
  }
  else{
    //TODO: Print message
  }
}

void MainWindow::on_linkblobbutton_clicked(){
  if(_currentStorage){
    std::vector<std::string> blobs=_currentStorage->getBlobs();
    BlobSelector bs(this);
    for(auto& elem: blobs){
      bs.append(elem);
    }
    int result=bs.exec();
    if(result==QDialog::Accepted){
      std::string selected_name=bs.get_selected_blob();
      add_blob_to_row(_reqtree->currentIndex(),selected_name);
      //std::cout << "selected_name " << selected_name << std::endl;
    }
  }
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

void MainWindow::on_reqmodel_item_changed(QStandardItem* item){
  //Commit new data to collection
  std::string model_node=get_uuid_by_modelindex(item->index());
  std::string model_text=get_text_by_modelindex(item->index());
  commit_to_collection(model_node,model_text);
  //std::cout << "on_reqmodel_item_changed " << model_node << " " << model_text << std::endl;
}

//Move node up one level. New parent is parent of parent.
//If there is no grandparent, we have the grandfather paradoxon ... not.
void MainWindow::on_reqtree_ctrl_left(const QModelIndex& i){
  std::string uuid=get_uuid_by_modelindex(i);
  if(uuid!=""){
    requirements::NodePtr node=get_node_for_uuid(uuid);
    requirements::NodePtr parent_node=node->getParent();
    if(parent_node!=nullptr){
      requirements::NodePtr grandparent_node=parent_node->getParent();
      if(grandparent_node!=nullptr){
        node->setLastOf(grandparent_node);
        //Jetzt Baum neu malen mit Springen zur UUID
        printtree(uuid);
      }
    }
  }
}

//Move a node down one level. New parent is the older brother.
//If there is no older brother, nothing changes.
void MainWindow::on_reqtree_ctrl_right(const QModelIndex& i){
  std::string uuid=get_uuid_by_modelindex(i);
  if(uuid!=""){
    requirements::NodePtr node=get_node_for_uuid(uuid);
    //wir brauchen die Liste aller Brüder
    //dazu fragen wie mal beim Vater nach der Geburtsurkunde
    //aber erst mal den Bengel nach seinem Vater fragen
    requirements::NodePtr parent_node=node->getParent();
    auto children=parent_node->getChildren();
    for(auto it=children.begin();it!=children.end();++it) {
      if(node==*it) {
        //found.
        if(it!=children.begin()){
          //er hat auch einen großen bruder:
          auto brother=it;
          --brother;
          //So, erst mal Schluck Glühwein nehmen
          //Jetzt gehts eklig weiter, jetzt wird nämlich der große Bruder zum Vater
          node->setLastOf(*brother);
          printtree(requirements::id_to_string(node->getId()));
        }
        break;
      }
    }
  }
}

//Move node up.
void MainWindow::on_reqtree_ctrl_up(const QModelIndex& i){
  std::string uuid=get_uuid_by_modelindex(i);
  if(uuid!=""){
    requirements::NodePtr node=get_node_for_uuid(uuid);
    node->up();
    //Jetzt den Knoten mit dem Vorgängerknoten gleicher Ebene im TreeModel vertauschen
    //brutale Methode: Printtree
    printtree(requirements::id_to_string(node->getId()));
  }
}

//Move node down
void MainWindow::on_reqtree_ctrl_down(const QModelIndex& i){
  std::string uuid=get_uuid_by_modelindex(i);
  if(uuid!=""){
    requirements::NodePtr node=get_node_for_uuid(uuid);
    node->down();
    //Jetzt den Knoten mit dem Vorgängerknoten gleicher Ebene im TreeModel vertauschen
    //brutale Methode: Printtree
    printtree(requirements::id_to_string(node->getId()));
  }
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
  _threadconnector.send_command(command);
}

}
