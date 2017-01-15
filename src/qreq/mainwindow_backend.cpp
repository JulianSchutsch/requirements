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


}
