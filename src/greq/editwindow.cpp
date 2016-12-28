#include "greq/editwindow.hpp"
#include <gtkmm/scrolledwindow.h>

namespace greq{

EditWindow::EditWindow(Glib::ustring text){

  set_default_size(640,480);
  set_border_width(10);

  //Add TextView
  _textview=Gtk::manage(new Gtk::TextView);

  Gtk::ScrolledWindow* scrolled=Gtk::manage(new Gtk::ScrolledWindow);
  scrolled->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  scrolled->add(*_textview);

  get_vbox()->pack_start(*scrolled);

  //add buttons
  add_button("OK",0);
  add_button("Cancel",1);

  //show all things
  _textview->show();
  scrolled->show();

  //fill content
  _textview->get_buffer()->set_text(text);
}

EditWindow::~EditWindow(){
}

Glib::ustring EditWindow::get_text(){
  return _textview->get_buffer()->get_text();
}

}
