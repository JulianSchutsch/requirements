#include "requirements/storage/text.hpp"
#include "requirements/select.hpp"

#include "greq/mainwindow.hpp"
#include "greq/settings.hpp"

#include <iostream>

namespace greq{

void MainWindow::init_collection(){
  _currentProject.reset(new ::requirements::storage::Text(Settings::getInstance().current_project, false));
}

void MainWindow::store_collection(){
  if(_currentProject) {
    _currentProject->save(Settings::getInstance().current_project);
  }
}

void MainWindow::printtree(){
  //Auf der Kommandozeile heißt das req folder
  using namespace ::requirements;

  std::vector<std::string> parameters; //Hier kann noch was sinnvolles rein.
  //now ignore the changed()-Signal
  ++_changed_signal_ignore;
  _left_tree_model->clear();

  auto& collection = _currentProject->getNodeCollection();
  auto selected = requirements::select(collection, parameters, collection.getRootNode());

  for(auto& node: selected){
    add_children_to_tree(nullptr,node);
  }

  _topictree->remove_all_columns();
  _topictree->append_column("topic", _topic_columns.col_node);
  _topictree->append_column_editable("text", _topic_columns.col_cont);
  //now do no longer ignore changed signal
  --_changed_signal_ignore;
}

requirements::NodePtr MainWindow::get_node_for_uuid(std::string const& uuid){

  std::vector<std::string> parameters;
  parameters.push_back(uuid);
  std::vector<requirements::NodePtr> selections;
  auto& collection = _currentProject->getNodeCollection();
  selections = requirements::select(collection, parameters);
  requirements::NodePtr node = selections[0];

  return node;
}

void MainWindow::commit_to_collection(std::string const& uuid, std::string const& content){
  std::vector<std::string> parameters; //Hier kommt die ID rein, oder?
  parameters.push_back(uuid);
  std::vector<requirements::NodePtr> selections;
  auto& collection = _currentProject->getNodeCollection();
  selections = requirements::select(collection, parameters);
  //Und, haben wir jetzt den passenden Knoten? Ein bisschen mehr Doku zum select()
  //wäre hilfreich
  if(selections.size()==1){
    //Oh, das ging ja...
    requirements::NodePtr node = selections[0];
    node->updateContent(content);
  }
}

void MainWindow::add_new_brother_for(std::string const& uuid){
  //Also erst mal im Req-Baum finden
  requirements::NodePtr parent=get_node_for_uuid(uuid)->getParent();
  //Jetzt neuen Knoten erzeugen
  auto& collection = _currentProject->getNodeCollection();
  auto newnode = collection.createNode("");
  //Jetzt Knoten unter den parent bammeln
  newnode->setParent(parent);
}

}
