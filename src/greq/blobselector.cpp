#include "greq/blobselector.hpp"
#include <gtkmm/scrolledwindow.h>

namespace greq{

//TODO: remove blobs
BlobSelector::BlobSelector(){

  set_default_size(640,480);
  set_border_width(10);

  //Add TreeView
  _bloblist=Gtk::manage(new Gtk::TreeView);

  //create blob model
  _blobmodel = Gtk::ListStore::create(_blob_columns);
  _bloblist->set_model(_blobmodel);
  _bloblist->set_reorderable();
  _bloblist->append_column("Name", _blob_columns.col_node);

  Gtk::ScrolledWindow* scrolled=Gtk::manage(new Gtk::ScrolledWindow);
  scrolled->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  scrolled->add(*_bloblist);

  get_vbox()->pack_start(*scrolled);

  //add buttons
  add_button("OK",0);
  add_button("Cancel",1);

  //show all things
  _bloblist->show();
  scrolled->show();
}

BlobSelector::~BlobSelector(){
}

std::string BlobSelector::get_selected_blob(){
  std::string retval="";
  Glib::RefPtr<Gtk::TreeSelection> selection = _bloblist->get_selection();
  Gtk::TreeModel::iterator selected_row = selection->get_selected();
  if(selected_row!=nullptr){
    Gtk::TreeModel::Row row = *selected_row;
    Glib::ustring retstr=row[_blob_columns.col_node];
    retval=retstr;
  }
  return retval;
}

void BlobSelector::append(std::string const& blob){

  Gtk::TreeModel::Row row=*(_blobmodel->append());
  row[_blob_columns.col_node] = blob;
}

}
