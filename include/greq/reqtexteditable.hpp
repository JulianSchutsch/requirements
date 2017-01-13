#pragma once

#include <gtkmm/celleditable.h>
#include <gtkmm/eventbox.h>
#include <gtkmm/textview.h>


typedef sigc::signal<void> signal_editing_done_t;

class ReqTextEditable : public Gtk::EventBox, public Gtk::CellEditable
{
protected:
  Glib::ustring _path;
  Gtk::TextView* _textview;
  signal_editing_done_t _signal_editing_done;

  // Override CellEditable
  virtual void start_editing_vfunc(GdkEvent* event);

  // Manage focus_out signal
  virtual bool on_focus_out(GdkEventFocus* gdk_event);
public:
  // Ctor/Dtor
  ReqTextEditable(const Glib::ustring& path);
  virtual ~ReqTextEditable();

  // Return creation path
  Glib::ustring get_path() const;

  // Get and set text
  void set_text(const Glib::ustring& text);
  Glib::ustring get_text() const;

  // Signal for editing done
  signal_editing_done_t& signal_editing_done() { return _signal_editing_done; };
};
