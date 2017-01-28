#include "qreq/mainwindow.hpp"
#include "qreq/settings.hpp"
#include "qreq/modelroles.hpp"

#include "requirements/storage/text.hpp"
#include "requirements/select.hpp"
#include "requirements/blob.hpp"

#include <iostream>

namespace qreq{

void MainWindow::init_project(){
  _currentStorage.reset(new ::requirements::storage::Text(Settings::getInstance().current_project, false));
}

void MainWindow::store_collection(){
  if(_currentStorage){
    _currentStorage->save(Settings::getInstance().current_project);
  }
}

void MainWindow::printtree(std::string const& uuid_to_jump){
  //Auf der Kommandozeile heißt das req folder
  using namespace ::requirements;

  std::vector<std::string> parameters; //Hier kann noch was sinnvolles rein.
  //now ignore the changed()-Signal
  //++_changed_signal_ignore;  //TODO anschließen
  _reqmodel->clear();

  auto& collection = _currentStorage->getNodeCollection();
  auto selected = requirements::select(collection, parameters, collection.getRootNode());

  QStandardItem *parentItem=_reqmodel->invisibleRootItem();
  for(auto& node: selected){
    add_children_to_tree(parentItem,node);
  }
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
    add_child_to_tree(parent_item,elem);
  }
}

//TODO: UUID nur noch aus ROLE_ID holen und irgendwann den item_id wegwerfen
//das würde auch zum setIndexWidget passen
void MainWindow::add_child_to_tree(QStandardItem *parent_item,const requirements::NodePtr& node){
  //++_changed_signal_ignore;   //TODO anschließen
  QStandardItem *item_text=new QStandardItem(QString(node->getContent().c_str()));  //TODO irgendwann entfällt das und es gibt nur noch die USER_ROLES
  item_text->setData(QVariant(QString(node->getContent().c_str())),Qt::UserRole + ROLE_TEXT);
  item_text->setData(QVariant(QString(requirements::id_to_string(node->getId()).c_str())),Qt::UserRole + ROLE_ID);
  item_text->setData(QVariant(QString("Einhornpfleger")),Qt::UserRole+ROLE_CAPTION);
  QStandardItem *item_id=new QStandardItem(QString(requirements::id_to_string(node->getId()).c_str()));
  QList<QStandardItem*> item;
  item << item_text << item_id;
  parent_item->appendRow(item);
  add_children_to_tree(item_text,node);
  //std::cout << item_text->data(Qt::UserRole + ROLE_ID).toString().toStdString() << std::endl;
  //--_changed_signal_ignore;   //TODO anschließen
}

requirements::NodePtr MainWindow::get_node_for_uuid(std::string const& uuid){

  std::vector<std::string> parameters;
  parameters.push_back(uuid);
  std::vector<requirements::NodePtr> selections;
  auto& collection = _currentStorage->getNodeCollection();
  selections = requirements::select(collection, parameters);
  requirements::NodePtr node = selections[0];

  return node;
}

void MainWindow::new_node(bool sibling,bool copy_content){
  //Erst mal die UUID des aktuellen Knotens herausfinden
  QModelIndex index=_reqtree->currentIndex();
  std::string uuid=get_uuid_by_modelindex(index);
  if(uuid!=""){
    //Jetzt neuen Knoten erzeugen
    auto& collection = _currentStorage->getNodeCollection();
    auto newnode = collection.createNode("");
    requirements::NodePtr oldnode=get_node_for_uuid(uuid);
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
}

void MainWindow::commit_to_collection(std::string const& uuid, std::string const& content){
  std::vector<std::string> parameters; //Hier kommt die ID rein, oder?
  parameters.push_back(uuid);
  std::vector<requirements::NodePtr> selections;
  auto& collection = _currentStorage->getNodeCollection();
  selections = requirements::select(collection, parameters);
  //Und, haben wir jetzt den passenden Knoten? Ein bisschen mehr Doku zum select()
  //wäre hilfreich
  if(selections.size()==1){
    //Oh, das ging ja...
    requirements::NodePtr node = selections[0];
    node->updateContent(content);
  }
}

std::string MainWindow::newblob(std::string sourcefilename){
  std::string id = requirements::importBlob(*_currentStorage, sourcefilename);
  return id;
}

}
