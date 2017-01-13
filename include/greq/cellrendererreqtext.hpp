#pragma once

#include <gdkmm.h>
#include <gtkmm/cellrenderertext.h>
//#include <gtkmm/cellrenderer.h>
#include <gtkmm/textview.h>

#include "greq/reqtexteditable.hpp"

namespace greq{

class CellRendererReqText : public Gtk::CellRendererText{
public:
  CellRendererReqText();
  virtual ~CellRendererReqText();

protected:
  // The custom CellEditable
  ReqTextEditable* _reqtext;

  // Override CellRendererText
  virtual Gtk::CellEditable* start_editing_vfunc(GdkEvent* event, Gtk::Widget& widget, const Glib::ustring& path, const Gdk::Rectangle& background_area, const Gdk::Rectangle& cell_area, Gtk::CellRendererState flags);

  // Override CellRendererText
  void on_editing_done();
};

}
