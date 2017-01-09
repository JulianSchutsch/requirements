#pragma once

#include <gtkmm/dialog.h>
#include <gtkmm/textview.h>

namespace greq{

class KeyCodeWindow : public Gtk::Dialog{
  Gtk::TextView* _textview;
  void fill_content();
  bool on_key_press(GdkEventKey *event);
public:
  KeyCodeWindow();
  virtual ~KeyCodeWindow();
};

}
