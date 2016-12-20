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
  std::cout << "F5" << std::endl;
}

void MainWindow::on_f6_clicked(){
  std::cout << "F6" << std::endl;
}

void MainWindow::on_f7_clicked(){
  std::cout << "F7" << std::endl;
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

  return true;
}


}