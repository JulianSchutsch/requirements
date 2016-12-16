#include <gtkmm/aboutdialog.h>
#include <gtkmm/filechooserdialog.h>

#include <gdk/gdkkeysyms-compat.h>

#include "greq/mainwindow.hpp"
#include "greq/settings.hpp"

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

}

void MainWindow::on_f6_clicked(){

}

void MainWindow::on_f7_clicked(){

}

void MainWindow::on_f8_clicked(){

}

void MainWindow::on_f10_clicked(){
  hide();
}

bool MainWindow::on_key_press(GdkEventKey *event){

  switch(event->keyval){
  case GDK_F1:
    on_f1_clicked();
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
    break;
  }

  return true;
}

}
