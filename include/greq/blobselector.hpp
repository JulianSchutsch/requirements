#pragma once

#include <gtkmm/dialog.h>
#include <gtkmm/treeview.h>
#include <gtkmm/treemodelcolumn.h>
#include <gtkmm/liststore.h>

namespace greq{

class BlobSelector : public Gtk::Dialog{
  Gtk::TreeView* _bloblist;

  //TreeModel for blob
  class BlobColumns : public Gtk::TreeModel::ColumnRecord{
  public:
    BlobColumns() { add(col_node);}
    Gtk::TreeModelColumn<Glib::ustring> col_node;
  };

  BlobColumns _blob_columns;
  Glib::RefPtr<Gtk::ListStore> _blobmodel;

public:
  BlobSelector();
  virtual ~BlobSelector();
  std::string get_selected_blob();
  void append(std::string const& blob);
};

}
