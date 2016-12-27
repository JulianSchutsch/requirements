#include <gtkmm/aboutdialog.h>
#include <gtkmm/filechooserdialog.h>

#include <gdk/gdkkeysyms-compat.h>

#include "greq/mainwindow.hpp"
#include "greq/settings.hpp"

#include "requirements/select.hpp"
//#include "requirements/id.hpp"

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

void MainWindow::on_f5_clicked(){
  std::cout << "F5" << std::endl;
}

void MainWindow::on_f6_clicked(){
  std::cout << "F6" << std::endl;
}

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
    auto newnode = _collection.createNode("");
    //Jetzt Knoten unter den parent bammeln
    newnode->setParent(parent);
    //Jetzt alle Kinder des Parent neu malen
    //Erst mal den Parent im Gtkmm-Baum finden

    Gtk::TreeModel::Path path=_topictree->get_model()->get_path(selected_row);
    if(path.up()==true){  //Häh? warum ist denn path.up() immer true, auch wenns kein parent gibt?
      //Wir haben einen Parent im Tree gefunden
      //Jetzt muss ich erst mal das row zum path finden.
      Gtk::TreeModel::iterator iter1 = _topictree->get_model()->get_iter(path);
      Gtk::TreeModel::Row parent_row = *iter1;

      remove_children_from_tree(&parent_row);
      ++_changed_signal_ignore;
      add_children_to_tree(&parent_row,parent);
      --_changed_signal_ignore;

      //Jetzt noch wieder an der Stelle parent_row aufklappen
      _topictree->expand_row(path,true);
    }
    else{
      //Es gibt keinen Vorgängerknoten.
      //Einen zusätzlichen neuen Knoten dranhängen.
      //Aus irgend einem Grund kommen wir hier gar nicht rein.
      add_children_to_tree(nullptr,parent);
    }
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
}

void MainWindow::on_ctrl_left(){
  std::cout << "[Ctrl]+[Left]" << std::endl;
}

void MainWindow::on_ctrl_down(){
  std::cout << "[Ctrl]+[Down]" << std::endl;
}

void MainWindow::on_ctrl_up(){
  std::cout << "[Ctrl]+[Up]" << std::endl;
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
