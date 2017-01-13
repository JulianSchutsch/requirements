#pragma once

#include <gdkmm.h>
#include <gtkmm/cellrenderertext.h>
//#include <gtkmm/cellrenderer.h>
#include <gtkmm/textview.h>

//#include "greq/reqtexteditable.hpp"

namespace greq{

class CellRendererReqText2 : public Gtk::CellRendererText{
public:
  CellRendererReqText2();
  virtual ~CellRendererReqText2();

protected:
  // The custom CellEditable
  Gtk::TextView* _textview;

  // Override CellRendererText
  virtual Gtk::CellEditable* start_editing_vfunc(GdkEvent* event, Gtk::Widget& widget, const Glib::ustring& path, const Gdk::Rectangle& background_area, const Gdk::Rectangle& cell_area, Gtk::CellRendererState flags);
  /*void render_vfunc (const Glib::RefPtr<Gdk::Drawable> &window,
                               Gtk::Widget                       &widget,
                               const Gdk::Rectangle              &background_area,
                               const Gdk::Rectangle              &cell_area,
                               const Gdk::Rectangle              &expose_area,
                               Gtk::CellRendererState             flags);
*/
  // Override CellRendererText
  //void on_editing_done();
  void editable_cb ();
};

}
