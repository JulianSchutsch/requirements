#include "requirements/storage/text.hpp"
#include "requirements/select.hpp"

#include "greq/mainwindow.hpp"
#include "greq/settings.hpp"

namespace greq{

void MainWindow::init_collection(){
  requirements::storage::Text storage(_collection, Settings::getInstance().current_project);
}

void MainWindow::printtree(){
  //Auf der Kommandozeile heißt das req folder
  using namespace ::requirements;

  std::vector<std::string> parameters; //Hier kann noch was sinnvolles rein.
  //now ignore the changed()-Signal
  _changed_signal_ignore=true;
  _left_tree_model->clear();

  auto selected = requirements::select(_collection, parameters, _collection.getRootNode());

  for(auto& node: selected){
    add_children_to_tree(nullptr,node);
  }

  _topictree->remove_all_columns();
  _topictree->append_column("topic", _topic_columns.col_node);
  _topictree->append_column_editable("text", _topic_columns.col_cont);
  //now do no longer ignore changed signal
  _changed_signal_ignore=false;
}

void MainWindow::commit_to_collection(std::string const& uuid, std::string const& content){
  std::vector<std::string> parameters; //Hier kommt die ID rein, oder?
  parameters.push_back(uuid);
  std::vector<requirements::NodePtr> selections;
  selections = requirements::select(_collection, parameters);
  //Und, haben wir jetzt den passenden Knoten? Ein bisschen mehr Doku zum select()
  //wäre hilfreich
  if(selections.size()==1){
    //Oh, das ging ja...
    requirements::NodePtr node = selections[0];
    node->updateContent(content);
  }
}

}
