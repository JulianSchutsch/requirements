#include "qreq/mainwindow.hpp"
#include "qreq/settings.hpp"

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

void MainWindow::add_child_to_tree(QStandardItem *parent_item,const requirements::NodePtr& node){
  //++_changed_signal_ignore;   //TODO anschließen
  QStandardItem *item_text=new QStandardItem(QString(node->getContent().c_str()));
  QStandardItem *item_id=new QStandardItem(QString(requirements::id_to_string(node->getId()).c_str()));
  QList<QStandardItem*> item;
  item << item_text << item_id;
  parent_item->appendRow(item);
  add_children_to_tree(item_text,node);
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

void MainWindow::new_node(bool copy_content){
  //Erst mal die UUID des aktuellen Knotens herausfinden
  QModelIndex index=_reqtree->currentIndex();
  std::string uuid=get_uuid_by_modelindex(index);
  if(uuid!=""){
    add_new_brother_for(uuid);
    //Der neue Knoten wird ein Bruder des aktuellen Knotens.
    requirements::NodePtr node=get_node_for_uuid(uuid);
    //Dazu brauchen wir also den Parent
    requirements::NodePtr parent=node->getParent();
    //Jetzt neuen Knoten erzeugen
    auto& collection = _currentStorage->getNodeCollection();
    auto newnode = collection.createNode("");
    //Jetzt Knoten unter den parent bammeln
    newnode->setLastOf(parent);
    if(copy_content==true){
      newnode->updateContent(node->getContent());
    }
    //Jetzt den Knoten in das Treemodel einfügen
    //hinter dem letzten child von parent
    QStandardItem* item_parent=get_parent_item_by_modelindex(index);
    add_child_to_tree(item_parent,newnode);
    //Focus dorthin
    set_focus_to_uuid(item_parent,requirements::id_to_string(newnode->getId()).c_str());
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

void MainWindow::add_new_brother_for(std::string const& uuid){
  //Also erst mal im Req-Baum finden
  requirements::NodePtr parent=get_node_for_uuid(uuid)->getParent();
  //Jetzt neuen Knoten erzeugen
  auto& collection = _currentStorage->getNodeCollection();
  auto newnode = collection.createNode("");
  //Jetzt Knoten unter den parent bammeln
  newnode->setLastOf(parent);
}

}
