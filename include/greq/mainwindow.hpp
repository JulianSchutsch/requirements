#pragma once

#include <gtkmm/button.h>
#include <gtkmm/menubutton.h>
#include <gtkmm/window.h>
#include <gtkmm/treeview.h>
#include <gtkmm/treemodelcolumn.h>
#include <gtkmm/treestore.h>

#include "requirements/node.hpp"
#include "requirements/nodecollection.hpp"
#include "requirements/istorage.hpp"

namespace greq{

class MainWindow : public Gtk::Window{
  //Member widgets:
  Gtk::Button* _f1_button;
  Gtk::Button* _f2_button;
  Gtk::Button* _f3_button;
  Gtk::Button* _f4_button;
  Gtk::Button* _f5_button;
  Gtk::Button* _f6_button;
  Gtk::Button* _f7_button;
  Gtk::Button* _f8_button;
  Gtk::Button* _f10_button;
  Gtk::TreeView* _topictree;
  Gtk::MenuButton* _recentbutton;

  //We need to ignore the changed signal over the treemodel if the tree
  //is filled out programmatically
  int _changed_signal_ignore;

  void add_child_to_tree(Gtk::TreeModel::Row* row,const requirements::NodePtr& node);
  void add_children_to_tree(Gtk::TreeModel::Row* row,const requirements::NodePtr& node);
  void remove_children_from_tree(Gtk::TreeModel::Row* row);
  void remove_children_from_tree(Gtk::TreeNodeChildren childs);
  requirements::NodePtr get_node_for_uuid(std::string const& uuid);
  void create_recent_menu();
  void set_current_project(std::string const& filename);
  void set_focus_to_uuid(Gtk::TreeModel::Row* parent,std::string const& uuid);

  //Signal handlers:
  void on_f1_clicked();
  void on_f2_clicked();
  void on_f3_clicked();
  void on_f4_clicked();
  void on_f5_clicked();
  void on_f6_clicked();
  void on_f7_clicked();
  void on_f8_clicked();
  void on_f10_clicked();
  void on_ctrl_right();
  void on_ctrl_left();
  void on_ctrl_down();
  void on_ctrl_up();
  bool on_key_press(GdkEventKey *event);
  void on_filename_selected(std::string const& filename);
  void on_topic_row_changed(const Gtk::TreeModel::Path& path, const Gtk::TreeModel::iterator& iter);

  //Backend functions
  std::unique_ptr<::requirements::IStorage> _currentProject;
  void init_collection();
  void store_collection();
  void printtree();
  void commit_to_collection(std::string const& uuid, std::string const& content);
  void add_new_brother_for(std::string const& uuid);

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
