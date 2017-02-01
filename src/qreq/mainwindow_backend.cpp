#include "qreq/mainwindow.hpp"
#include "qreq/settings.hpp"
#include "qreq/model.hpp"

#include "requirements/select.hpp"
#include "requirements/blob.hpp"

#include <iostream>

namespace qreq{

void MainWindow::printtree(std::string const& uuid_to_jump){
  using namespace ::requirements;

  //now ignore the changed()-Signal
  //++_changed_signal_ignore;  //TODO anschließen
  _reqmodel->clear();

  if(modelState.nodeCollection==nullptr) {
    return;
  }
  auto& collection = *modelState.nodeCollection;

  QStandardItem *parentItem=_reqmodel->invisibleRootItem();
  add_children_to_tree(_reqmodel->invisibleRootItem(), collection.getRootNode());
  _reqtree->resizeColumnToContents(COLUMN_TEXT);
  _reqtree->hideColumn(COLUMN_ID);

  //--_changed_signal_ignore;  //TODO anschließen
  //Tree ausklappen
  _reqtree->expandAll();
  //Jetzt noch hinspringen
  if(uuid_to_jump!="") set_focus_to_uuid(parentItem,uuid_to_jump);
}

void MainWindow::add_children_to_tree(QStandardItem *parent_item,const requirements::NodePtr& node){
  auto& children=node->getChildren();
  for(auto& elem: children){
    std::cout<<"Child?"<<::requirements::id_to_string(elem->getId())<<std::endl;
    add_child_to_tree(parent_item,elem);
  }
}

//TODO: UUID nur noch aus ROLE_ID holen und irgendwann den item_id wegwerfen
//das würde auch zum setIndexWidget passen
void MainWindow::add_child_to_tree(QStandardItem *parent_item,const requirements::NodePtr& node){
  //++_changed_signal_ignore;   //TODO anschließen
  QStandardItem *item_text=new QStandardItem(QString(node->getContent().c_str()));  //TODO irgendwann entfällt das und es gibt nur noch die USER_ROLES
  ModelItem mItem(modelState, node->getId());
  mItem.saveToQStandardItem(*item_text);
  QList<QStandardItem*> item;
  item << item_text;
  parent_item->appendRow(item);
  add_children_to_tree(item_text,node);
  //--_changed_signal_ignore;   //TODO anschließen
}

void MainWindow::new_node(bool sibling,bool copy_content){
  if(modelState.nodeCollection==nullptr) {
    return;
  }
  QModelIndex index=_reqtree->currentIndex();
  ModelItem mItem(index);
  auto& collection = *modelState.nodeCollection;
  auto newnode = collection.createNode("");
  requirements::NodePtr oldnode=collection.getNodeById(mItem.getId());
  if(sibling==true){
    //Der neue Knoten wird ein Bruder des aktuellen Knotens
    newnode->setNextTo(oldnode);
  }
  else{
    //Der neue Knoten wird ein Kind des aktuellen Knotens
    newnode->setLastOf(oldnode);
  }
  if(copy_content==true){
    newnode->updateContent(oldnode->getContent());
  }
  //Und jetzt ab auf den Bildschirm
  printtree(requirements::id_to_string(newnode->getId()).c_str());
}

void MainWindow::commit_to_collection(std::string const& uuid, std::string const& content){
  std::vector<std::string> parameters; //Hier kommt die ID rein, oder?
  parameters.push_back(uuid);
  std::vector<requirements::NodePtr> selections;
  if(modelState.nodeCollection==nullptr) {
    return;
  }
  auto& collection = *modelState.nodeCollection;
  ::requirements::Id id;
  if(!::requirements::string_to_id(uuid, id)) {
    return;
  }
  auto node = collection.getNodeById(id);
  if(node) {
    node->updateContent(content);
  }
}

std::string MainWindow::newblob(std::string sourcefilename){
  // TODO: Send Command instead
  //std::string id = requirements::importBlob(*_currentStorage, sourcefilename);
  return "";
}

}
