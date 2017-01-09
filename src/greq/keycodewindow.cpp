#include "greq/keycodewindow.hpp"

#include <gtkmm/scrolledwindow.h>
#include <gdk/gdkkeysyms-compat.h>

namespace greq{

  const char *keycodecontent =
  "F1: About\n"
  "F2: Save\n"
  "F4: Edit\n"
  "F5: Copy\n"
  "F7: New\n"
  "F8: Delete\n"
  "F9: Keycodes\n"
  "F10: Exit\n"
  "Ctrl+left: Move node up one level\n"
  "Ctrl+right: Move node down one level\n"
  "Ctrl+up: Move node up\n"
  "Ctrl+down: Move node down\n";



KeyCodeWindow::KeyCodeWindow(){

  set_default_size(640,480);
  set_border_width(10);

  //Add TextView
  _textview=Gtk::manage(new Gtk::TextView);

  Gtk::ScrolledWindow* scrolled=Gtk::manage(new Gtk::ScrolledWindow);
  scrolled->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  scrolled->add(*_textview);

  get_vbox()->pack_start(*scrolled);

  //add buttons
  add_button("Close",0);

  //Connect Signals
  this->signal_key_press_event().connect(sigc::mem_fun(*this, &KeyCodeWindow::on_key_press),false);

  //show all things
  _textview->show();
  scrolled->show();

  //fill content
  fill_content();
}

KeyCodeWindow::~KeyCodeWindow(){
}

void KeyCodeWindow::fill_content(){
  _textview->get_buffer()->set_text(keycodecontent);
}

bool KeyCodeWindow::on_key_press(GdkEventKey *event){
  if((event->keyval==GDK_Escape)||(event->keyval==GDK_Return)) hide();
  return true;
}

}
