#include "greq/reqtexteditable.hpp"

#include <gtkmm.h>  //TODO nur die benötigten Sachen
#include <gtk/gtktextview.h>

#include <sstream>

ReqTextEditable::ReqTextEditable(const Glib::ustring& path) :
  Glib::ObjectBase( typeid(ReqTextEditable) ),
  Gtk::EventBox(),
  Gtk::CellEditable(),
  _path( path ),
  _textview( 0 )
{

  _textview=Gtk::manage(new Gtk::TextView);
  //_textview->set_has_frame(false);
  //_textview->gobj()->is_cell_renderer = true;

  Gtk::Box *vbox=Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL));
  vbox->pack_start (*_textview, Gtk::PACK_EXPAND_WIDGET);

  //set_flags(Gtk::CAN_FOCUS);

  _textview->show();
  vbox->show();
}

ReqTextEditable::~ReqTextEditable()
{
}

Glib::ustring ReqTextEditable::get_path() const
{
  return _path;
}

void ReqTextEditable::set_text(const Glib::ustring& text)
{
  _textview->get_buffer()->set_text(text);
}

Glib::ustring ReqTextEditable::get_text() const
{
  return _textview->get_buffer()->get_text();
}

void ReqTextEditable::start_editing_vfunc(GdkEvent*)
{
    //_textview->select_region(0, -1);
  _textview->signal_focus_out_event().connect(sigc::mem_fun(*this,&ReqTextEditable::on_focus_out));
}

bool ReqTextEditable::on_focus_out(GdkEventFocus* gdk_event)
{
  (void)gdk_event;
  _signal_editing_done.emit();
  return true;
}
