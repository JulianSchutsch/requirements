#include "qreq/mainwindow.hpp"
#include "qreq/settings.hpp"

#include "requirements/storage/text.hpp"
#include "requirements/select.hpp"
#include "requirements/blob.hpp"

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
  //_left_tree_model->clear(); //TODO anschließen

  auto& collection = _currentStorage->getNodeCollection();
  auto selected = requirements::select(collection, parameters, collection.getRootNode());

  QStandardItem *parentItem=_reqmodel->invisibleRootItem();
  for(auto& node: selected){
    add_children_to_tree(parentItem,node);
  }
  _reqtree->resizeColumnToContents(0);

  //_topictree->remove_all_columns();   //TODO anschließen
  //_topictree->append_column("topic", _topic_columns.col_node);  //TODO anschließen
  //_topictree->append_column_editable("text", _topic_columns.col_cont);  //TODO anschließen

  //--_changed_signal_ignore;  //TODO anschließen
  //Tree ausklappen
  //_topictree->expand_all();  //TODO anschließen
  //Jetzt noch hinspringen
  //if(uuid_to_jump!="") set_focus_to_uuid(uuid_to_jump);  //TODO anschließen
}

void MainWindow::add_children_to_tree(QStandardItem *parent_item,const requirements::NodePtr& node){
  auto& children=node->getChildren();
  for(auto& elem: children){
    add_child_to_tree(parent_item,elem);
  }
}

void MainWindow::add_child_to_tree(QStandardItem *parent_item,const requirements::NodePtr& node){
  //++_changed_signal_ignore;   //TODO anschließen
  //QStandardItem *item=new QStandardItem(QString(requirements::id_to_string(node->getId()).c_str()));
  QStandardItem *item=new QStandardItem(QString(node->getContent().c_str()));
  parent_item->appendRow(item);
  add_children_to_tree(item,node);
  //--_changed_signal_ignore;   //TODO anschließen
}

}
