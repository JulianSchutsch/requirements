#pragma once

#include <gtkmm/button.h>
#include <gtkmm/menubutton.h>
#include <gtkmm/window.h>
#include <gtkmm/treeview.h>
#include <gtkmm/treemodelcolumn.h>
#include <gtkmm/treestore.h>

#include "requirements/node.hpp"
#include "requirements/nodecollection.hpp"

namespace greq{

class MainWindow : public Gtk::Window{
  //Member widgets:
  Gtk::Button* _f1_button;
  Gtk::Button* _f2_button;
  Gtk::Button* _f3_button;
  Gtk::Button* _f10_button;
  Gtk::TreeView* _topictree;
  Gtk::MenuButton* _recentbutton;

  //We need to ignore the changed signal over the treemodel if the tree
  //is filled out programmatically
  bool _changed_signal_ignore;

  void add_child_to_tree(Gtk::TreeModel::Row* row,const requirements::NodePtr& node);
  void add_children_to_tree(Gtk::TreeModel::Row* row,const requirements::NodePtr& node);
  void create_recent_menu();
  void set_current_project(std::string const& filename);

  //Signal handlers:
  void on_f1_clicked();
  void on_f2_clicked();
  void on_f3_clicked();
  void on_f10_clicked();
  bool on_key_press(GdkEventKey *event);
  void on_filename_selected(std::string const& filename);
  void on_topic_row_changed(const Gtk::TreeModel::Path& path, const Gtk::TreeModel::iterator& iter);

  //Backend functions
  requirements::NodeCollection _collection;
  void init_collection();
  void printtree();
  void commit_to_collection(std::string const& uuid, std::string const& content);

  //TreeModel for left topictree
  class TopicColumns : public Gtk::TreeModel::ColumnRecord{
  public:
    TopicColumns() { add(col_node);add(col_cont); }
    Gtk::TreeModelColumn<Glib::ustring> col_node;
    Gtk::TreeModelColumn<Glib::ustring> col_cont;
  };

  TopicColumns _topic_columns;
  Glib::RefPtr<Gtk::TreeStore> _left_tree_model;

public:
  MainWindow();
  virtual ~MainWindow();
};


}
