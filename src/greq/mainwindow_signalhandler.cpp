#include <gtkmm/aboutdialog.h>
#include <gtkmm/filechooserdialog.h>

#include <gdk/gdkkeysyms-compat.h>

#include "greq/mainwindow.hpp"
#include "greq/editwindow.hpp"
#include "greq/settings.hpp"

#include "requirements/select.hpp"
#include "requirements/id.hpp"

#include <iostream>

namespace greq{

void MainWindow::on_f1_clicked()
{
  Gtk::AboutDialog about;
  about.set_transient_for(*this);
  about.set_program_name("Über");
  about.set_version("0.0.0");
  about.set_website("https://github.com/JulianSchutsch/requirements");
  std::vector<Glib::ustring> list_authors;
  list_authors.push_back("Julian Schutsch");
  list_authors.push_back("Dirk Neumann");
  about.set_authors(list_authors);

  about.show();
  about.run();
}

void MainWindow::on_filename_selected(std::string const& filename){
  set_current_project(filename);
}

void MainWindow::on_f2_clicked(){
  store_collection();
}

void MainWindow::on_f3_clicked(){
  //Select root path by dialog
  Gtk::FileChooserDialog filedlg(*this,"Select project",Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);
  filedlg.set_transient_for(*this);
  filedlg.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
  filedlg.add_button("Select", Gtk::RESPONSE_OK);
  int result=filedlg.run();
  switch(result){
  case Gtk::RESPONSE_OK:
    Settings::getInstance().add_project(filedlg.get_filename());
    set_current_project(filedlg.get_filename());
    create_recent_menu();
    break;
  case Gtk::RESPONSE_CANCEL:
    break;
  default:
    //this is for some strange behavior in terms of unknown button code
    break;
  }
}

void MainWindow::on_f4_clicked(){
  //Finden, ob ein echter Knoten drunter liegt
  Glib::RefPtr<Gtk::TreeSelection> selection = _topictree->get_selection();
  Gtk::TreeModel::iterator selected_row = selection->get_selected();
  if(selected_row!=nullptr){
    //Text des Knotens raussuchen und in den Dialog quetschen
    Gtk::TreeModel::Row row = *selected_row;
    Glib::ustring content = row[_topic_columns.col_cont];
    Glib::ustring uuid = row[_topic_columns.col_node];
    EditWindow ew(content);
    ew.set_transient_for(*this);
    int result=ew.run();
    switch(result){
    case 0:{
      //Text auslesen
      Glib::ustring read_text=ew.get_text();
      //In den Baum schreiben, das changed()-Signal sorgt dafür, dass das in die collection kommt
      row[_topic_columns.col_cont]=read_text;
    }
    break;
    case 1:
    default:
      break;
    }
  }
}

void MainWindow::on_f5_clicked(){
  std::cout << "F5" << std::endl;
}

void MainWindow::on_f6_clicked(){
  std::cout << "F6" << std::endl;
}

//TODO das sieht ein bisschen unaufgeräumt aus...
void MainWindow::on_f7_clicked(){
  //Erst mal die UUID des aktuellen Knotens herausfinden
  Glib::RefPtr<Gtk::TreeSelection> selection = _topictree->get_selection();
  Gtk::TreeModel::iterator selected_row = selection->get_selected();
  if(selected_row!=nullptr){
    Gtk::TreeModel::Row row = *selected_row;
    Glib::ustring uuid = row[_topic_columns.col_node];
    //Der neue Knoten wird ein Bruder des aktuellen Knotens.
    //Dazu brauchen wir also den Parent
    requirements::NodePtr parent=get_node_for_uuid(uuid)->getParent();
    //Jetzt neuen Knoten erzeugen
    auto& collection = _currentProject->getNodeCollection();
    auto newnode = collection.createNode("");
    //Jetzt Knoten unter den parent bammeln
    newnode->setParent(parent);
    //Jetzt den Knoten einfügen hinter dem letzten child von parent
    //Dazu erst mal den parent finden
    Gtk::TreeModel::Path path=_left_tree_model->get_path(*row);
    if(path.up()==true){
      Gtk::TreeModel::iterator parent_iter = _left_tree_model->get_iter(path);
      //Gtk::TreeModel::Row parent_row = *parent_iter;
      Gtk::TreeModel::iterator newchild_iter=_left_tree_model->append(parent_iter->children());
      //Inhalt reinschreiben
      (*newchild_iter)[_topic_columns.col_node] = requirements::id_to_string(newnode->getId());
      (*newchild_iter)[_topic_columns.col_cont] = newnode->getContent();
      //Und jetzt den Focus setzen
      _topictree->set_cursor(_left_tree_model->get_path(*(*newchild_iter)));
    }

    //Jetzt alle Kinder des Parent neu malen
    //reprint_tree_below_parent_of(&row,requirements::id_to_string(newnode->getId()));
  }
}

void MainWindow::on_f8_clicked(){
  std::cout << "F8" << std::endl;
}

void MainWindow::on_f10_clicked(){
  hide();
}

void MainWindow::on_ctrl_right(){
  std::cout << "[Ctrl]+[Right]" << std::endl;
  //Im Branch full_iterate nachschauen: Reparenting der collection, und dann den Tree komplett neu malen
  //mit dem universellen set_focus_to_uuid(string)
}

void MainWindow::on_ctrl_left(){
  std::cout << "[Ctrl]+[Left]" << std::endl;
}

//TODO Die ersten paar Zeilen kann man zu einer eigenen methode machen: get_uuid_on_cursor oder so...
//Na, dann braucht man noch das &row. Aber das ist bis jetzt immer der aktuelle Cursor gewesen.
void MainWindow::on_ctrl_down(){
  //Erst mal die UUID des aktuellen Knotens herausfinden
  Glib::RefPtr<Gtk::TreeSelection> selection = _topictree->get_selection();
  Gtk::TreeModel::iterator selected_row = selection->get_selected();
  if(selected_row!=nullptr){
    //Move Node down
    Gtk::TreeModel::Row row = *selected_row;
    Glib::ustring uuid = row[_topic_columns.col_node];
    requirements::NodePtr node=get_node_for_uuid(uuid);
    node->down();
    //Jetzt den Knoten mit dem Nachfolgeknoten gleicher Ebene vertauschen
    Gtk::TreeModel::iterator successor_row=selected_row;
    ++successor_row;
    _left_tree_model->iter_swap(selected_row,successor_row);
  }
}

void MainWindow::on_ctrl_up(){
  //Erst mal die UUID des aktuellen Knotens herausfinden
  Glib::RefPtr<Gtk::TreeSelection> selection = _topictree->get_selection();
  Gtk::TreeModel::iterator selected_row = selection->get_selected();
  if(selected_row!=nullptr){
    //Move Node up
    Gtk::TreeModel::Row row = *selected_row;
    Glib::ustring uuid = row[_topic_columns.col_node];
    requirements::NodePtr node=get_node_for_uuid(uuid);
    node->up();
    //Jetzt den Knoten mit dem Vorgängerknoten gleicher Ebene vertauschen
    Gtk::TreeModel::iterator ancestor_row=selected_row;
    --ancestor_row;
    //iter_is_valid is slow. Is there a faster alternative?
    if(_left_tree_model->iter_is_valid(ancestor_row)==true) _left_tree_model->iter_swap(selected_row,ancestor_row);
  }
}

bool MainWindow::on_key_press(GdkEventKey *event){

  std::cout << "state: " << std::hex << event->state << std::endl;
  if(event->state==0x14){
    //Hm, Ctrl-Key
    switch(event->keyval){
    case GDK_Right:
      on_ctrl_right();
      break;
    case GDK_Down:
      on_ctrl_down();
      break;
    case GDK_Up:
      on_ctrl_up();
      break;
    case GDK_Left:
      on_ctrl_left();
      break;
    default:
      std::cout << "Ctrl+ " << std::hex << event->keyval << std::endl;
      break;
    }
  }
  else{
    //No Ctrl-Key
    switch(event->keyval){
    case GDK_F1:
      on_f1_clicked();
      break;
    case GDK_F2:
      on_f2_clicked();
      break;
    case GDK_F3:
      on_f3_clicked();
      break;
    case GDK_F4:
      on_f4_clicked();
      break;
    case GDK_F5:
      on_f5_clicked();
      break;
    case GDK_F6:
      on_f6_clicked();
      break;
    case GDK_F7:
      on_f7_clicked();
      break;
    case GDK_F8:
      on_f8_clicked();
      break;
    case GDK_F10:
      on_f10_clicked();
    break;
    default:
      std::cout << "keycode " << std::hex << event->keyval << std::endl;
      //add event to keystack
      break;
    }
  }

  return false; //Event propagation überdenken: In speziellen Edit-Modes vielleicht true (d.h. Event hier schlucken) zurückgeben?
}

void MainWindow::on_topic_row_changed(const Gtk::TreeModel::Path& path, const Gtk::TreeModel::iterator& iter){
  (void)path;
  if(_changed_signal_ignore<1){
    if(iter){
      //Dann muss es jetzt committed werden
      Gtk::TreeModel::Row row = *iter;
      Glib::ustring model_value = row[_topic_columns.col_cont];
      Glib::ustring model_node = row[_topic_columns.col_node];

      commit_to_collection(model_node,model_value);
    }
  }
}

}
