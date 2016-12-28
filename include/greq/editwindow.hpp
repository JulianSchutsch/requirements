#pragma once

#include <gtkmm/dialog.h>
#include <gtkmm/textview.h>

namespace greq{

class EditWindow : public Gtk::Dialog{
  Gtk::TextView* _textview;

public:
  EditWindow(Glib::ustring text);
  virtual ~EditWindow();
  Glib::ustring get_text();
};

}
