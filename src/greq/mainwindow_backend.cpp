#include "requirements/storage/text.hpp"
#include "requirements/select.hpp"
#include "requirements/blob.hpp"

#include "greq/mainwindow.hpp"
#include "greq/settings.hpp"
#include "greq/cellrendererreqtext.hpp"
#include "greq/cellrendererreqtext2.hpp"

#include <iostream>

namespace greq{

void MainWindow::init_project(){
  _currentStorage.reset(new ::requirements::storage::Text(Settings::getInstance().current_project, false));
}

void MainWindow::store_collection(){
  if(_currentStorage){
    _currentStorage->save(Settings::getInstance().current_project);
  }
}

void MainWindow::cellrenderer_reqtext_on_editing_started( Gtk::CellEditable* cell_editable, const Glib::ustring& /* path */)
{
  std::cout << "on_editing_started" << std::endl;
}

void MainWindow::printtree(std::string const& uuid_to_jump){
  //Auf der Kommandozeile heißt das req folder
  using namespace ::requirements;

  std::vector<std::string> parameters; //Hier kann noch was sinnvolles rein.
  //now ignore the changed()-Signal
  ++_changed_signal_ignore;
  _left_tree_model->clear();

  auto& collection = _currentStorage->getNodeCollection();
  auto selected = requirements::select(collection, parameters, collection.getRootNode());

  for(auto& node: selected){
    add_children_to_tree(nullptr,node);
  }

  _topictree->remove_all_columns();
  //_topictree->append_column("topic", _topic_columns.col_node);

  //Basteln des Cellrenderers und der ViewColumn
  //Gtk::TreeViewColumn* view_column = Gtk::manage(new Gtk::TreeViewColumn(("text")));
  //CellRendererReqText* reqtextrenderer=Gtk::manage(new CellRendererReqText);
  //reqtextrenderer->property_editable() = true;
  //view_column->pack_start(*reqtextrenderer, false);
  //_topictree->append_column(*view_column);
  //Make the CellRenderer editable, and handle its editing signals:
  //Jetzt müssen wir die passende modelcolumn zuweisen

  //alternativ:
  CellRendererReqText2* reqtextrenderer=Gtk::manage(new CellRendererReqText2);
  reqtextrenderer->property_editable() = true;
  Gtk::TreeViewColumn* view_column2 = Gtk::manage(new Gtk::TreeViewColumn(Glib::ustring("text"),*reqtextrenderer));
  view_column2->add_attribute(*reqtextrenderer,"text",1);
  _topictree->append_column(*view_column2);
  reqtextrenderer->signal_editing_started().connect(sigc::mem_fun(*this, &MainWindow::cellrenderer_reqtext_on_editing_started) );


  //_topictree->append_column("topic", _topic_columns.col_node);
  //_topictree->append_column_editable("text", _topic_columns.col_cont);
  //Original
  //_topictree->append_column_editable("text", _topic_columns.col_cont);
  //Ende Original


  //CellRendererReqText* reqtextrenderer=Gtk::manage(new CellRendererReqText);
  //_topictree->get_column(0)->set_renderer(*reqtextrenderer,_topic_columns.col_cont);
  //Gtk::CellRendererText* cellrenderer=dynamic_cast<Gtk::CellRendererText*>(_topictree->get_column_cell_renderer(0));
  //cellrenderer->property_editable()=true;

  //now do no longer ignore changed signal
  --_changed_signal_ignore;
  //Tree ausklappen
  _topictree->expand_all();
  //Jetzt noch hinspringen
  if(uuid_to_jump!="") set_focus_to_uuid(uuid_to_jump);
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

void MainWindow::add_new_brother_for(std::string const& uuid){
  //Also erst mal im Req-Baum finden
  requirements::NodePtr parent=get_node_for_uuid(uuid)->getParent();
  //Jetzt neuen Knoten erzeugen
  auto& collection = _currentStorage->getNodeCollection();
  auto newnode = collection.createNode("");
  //Jetzt Knoten unter den parent bammeln
  newnode->setLastOf(parent);
}

void MainWindow::new_node(bool copy_content){
  //Erst mal die UUID des aktuellen Knotens herausfinden
  Glib::RefPtr<Gtk::TreeSelection> selection = _topictree->get_selection();
  Gtk::TreeModel::iterator selected_row = selection->get_selected();
  if(selected_row!=nullptr){
    Gtk::TreeModel::Row row = *selected_row;
    Glib::ustring uuid = row[_topic_columns.col_node];
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
    //Jetzt den Knoten einfügen hinter dem letzten child von parent
    //Dazu erst mal den parent finden
    Gtk::TreeModel::Path path=_left_tree_model->get_path(*row);
    if(path.up()==true){
      Gtk::TreeModel::iterator parent_iter = _left_tree_model->get_iter(path);
      Gtk::TreeModel::iterator newchild_iter=_left_tree_model->append(parent_iter->children());
      //Inhalt reinschreiben
      ++_changed_signal_ignore;
      (*newchild_iter)[_topic_columns.col_node] = requirements::id_to_string(newnode->getId());
      (*newchild_iter)[_topic_columns.col_cont] = newnode->getContent();
      --_changed_signal_ignore;
      //Und jetzt den Focus setzen
      _topictree->set_cursor(_left_tree_model->get_path(*(*newchild_iter)));
    }
  }
}

std::string MainWindow::newblob(std::string sourcefilename){
  std::string id = requirements::importBlob(*_currentStorage, sourcefilename);
  //if(id.empty()) {
  //  std::cout << "Failed to import blob" << std::endl;
  //}
  //else{
  //  std::cout<<"Blob added: "<<id<<std::endl;
  //}
  return id;
}


}
