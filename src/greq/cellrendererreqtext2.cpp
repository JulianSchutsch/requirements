#include "greq/cellrendererreqtext2.hpp"

#include <iostream>
#include <gtkmm.h>

namespace greq{

CellRendererReqText2::CellRendererReqText2()
{
  _textview=Gtk::manage(new Gtk::TextView);

  Gtk::Box *vbox=Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL));
  vbox->pack_start (*_textview, Gtk::PACK_EXPAND_WIDGET);

  _textview->show();
  vbox->show();
  property_editable ().signal_changed ().connect (sigc::mem_fun (*this, &CellRendererReqText2::editable_cb));
}

CellRendererReqText2::~CellRendererReqText2(){
}

void CellRendererReqText2::editable_cb ()
{
    property_mode () = property_editable () ? Gtk::CELL_RENDERER_MODE_EDITABLE : Gtk::CELL_RENDERER_MODE_INERT;
}

/*void CellRendererReqText2::render_vfunc (const Glib::RefPtr<Gdk::Drawable> &window,
                                         Gtk::Widget                       &widget,
                                         const Gdk::Rectangle              &background_area,
                                         const Gdk::Rectangle              &cell_area,
                                         const Gdk::Rectangle              &expose_area,
                                         Gtk::CellRendererState             flags)
{
    // This ugly cast is necessary to work around a bug in GTKmm 2.4
    const Glib::RefPtr<Gdk::Window> &window_win = reinterpret_cast<const Glib::RefPtr<Gdk::Window>&> (window);

    int pix_x, pix_y, pix_w, pix_h;
    cell_pixbuf.get_size (widget, cell_area, pix_x, pix_y, pix_w, pix_h);

    Gdk::Rectangle pix_cell_area = cell_area;
    pix_cell_area.set_width (pix_w);

    cell_pixbuf.render (window_win, widget, background_area, pix_cell_area, expose_area, flags);

    Gdk::Rectangle text_cell_area = cell_area;

    if (pix_w)
    {
        text_cell_area.set_x (text_cell_area.get_x () + pix_w + property_xpad ());
        text_cell_area.set_width (text_cell_area.get_width () - pix_w - property_xpad ());
    }

    Gtk::CellRendererText::render_vfunc (window, widget, background_area, text_cell_area, expose_area, flags);
}*/

Gtk::CellEditable* CellRendererReqText2::start_editing_vfunc(GdkEvent* event, Gtk::Widget& widget, const Glib::ustring& path, const Gdk::Rectangle& background_area, const Gdk::Rectangle& cell_area, Gtk::CellRendererState flags)
{
  std::cout << "start_editing_vfunc" << std::endl;
// If the cell isn't editable we return 0.
#ifdef GLIBMM_PROPERTIES_ENABLED
  if (!property_editable())
    return 0;
#else
  if (!(g_object_get_data(G_OBJECT(gobj()), "editable")))
    return 0;
#endif
  std::cout << "weiter_editing_vfunc" << std::endl;

  Glib::ustring text;

#ifdef GLIBMM_PROPERTIES_ENABLED
  text = property_text();
#else
  get_property("text", text);
#endif

  _textview->get_buffer()->set_text(text);
   //_reqtext->set_text (text);
  //_textview->signal_editing_done().connect(sigc::mem_fun(*this, &CellRendererReqText::on_editing_done));
  //_textview->show();

  //_reqtext = Gtk::manage( _reqtext.release() );
  std::cout << "done_editing_vfunc" << std::endl;
  return Gtk::CellRenderer::start_editing_vfunc (event, widget, path, background_area, cell_area, flags);
}


//void CellRendererReqText2::on_editing_done()
//{
//  std::cout << "on_editing_done" << std::endl;
//  edited (_reqtext->get_path(), _reqtext->get_text());
//}

}
