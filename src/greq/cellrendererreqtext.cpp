#include "greq/cellrendererreqtext.hpp"

#include <iostream>

namespace greq{

CellRendererReqText::CellRendererReqText() :
    Glib::ObjectBase( typeid(CellRendererReqText) ),
    Gtk::CellRendererText(),
    _reqtext( 0 )
{
}

CellRendererReqText::~CellRendererReqText(){
}


Gtk::CellEditable* CellRendererReqText::start_editing_vfunc(GdkEvent* event, Gtk::Widget& widget, const Glib::ustring& path, const Gdk::Rectangle& background_area, const Gdk::Rectangle& cell_area, Gtk::CellRendererState flags)
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
  std::shared_ptr< ReqTextEditable > _reqtext( new ReqTextEditable( path ) );

  Glib::ustring text;

#ifdef GLIBMM_PROPERTIES_ENABLED
  text = property_text();
#else
  get_property("text", text);
#endif

  _reqtext->set_text (text);
  _reqtext->signal_editing_done().connect(sigc::mem_fun(*this, &CellRendererReqText::on_editing_done));
  _reqtext->show();

  //_reqtext = Gtk::manage( _reqtext.release() );
  std::cout << "done_editing_vfunc" << std::endl;
  //return &*_reqtext;
  return Gtk::CellRenderer::start_editing_vfunc (event, widget, path, background_area, cell_area, flags);
}


void CellRendererReqText::on_editing_done()
{
  std::cout << "on_editing_done" << std::endl;
  edited (_reqtext->get_path(), _reqtext->get_text());
}

}
