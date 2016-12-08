#pragma once

#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm/treeview.h>
#include <gtkmm/treemodelcolumn.h>
#include <gtkmm/treestore.h>
#include <gtkmm/liststore.h>

#include "requirements/node.hpp"

class MainWindow : public Gtk::Window{
  //Member widgets:
  Gtk::Button* _f1_button;
  Gtk::Button* _f2_button;
  Gtk::Button* _f3_button;
  Gtk::Button* _f10_button;
  Gtk::TreeView* _topictree;
  Gtk::TreeView* _contenttree;

  void fill_with_dull_data();
  void printtree(std::string dirname);
  void add_child_to_tree(Gtk::TreeModel::Row* row,const requirements::NodePtr& node);
  void add_children_to_tree(Gtk::TreeModel::Row* row,const requirements::NodePtr& node);
  //Signal handlers:
  void on_f1_clicked();
  void on_f2_clicked();
  void on_f3_clicked();
  void on_f10_clicked();
  bool on_key_press(GdkEventKey *event);

  //TreeModel for left topictree
  class TopicColumns : public Gtk::TreeModel::ColumnRecord{
  public:
    TopicColumns() { add(col_node);add(col_cont); }
    Gtk::TreeModelColumn<Glib::ustring> col_node;
    Gtk::TreeModelColumn<Glib::ustring> col_cont;
  };

  TopicColumns _topic_columns;
  Glib::RefPtr<Gtk::TreeStore> _left_tree_model;

  //TreeModel for right content list
  class ContentColumns : public Gtk::TreeModel::ColumnRecord{
  public:
    ContentColumns()
    { add(col_id); add(col_text);}

    Gtk::TreeModelColumn<unsigned int> col_id;
    Gtk::TreeModelColumn<Glib::ustring> col_text;
  };

  ContentColumns _content_columns;
  Glib::RefPtr<Gtk::ListStore> _right_list_model;

public:
  MainWindow();
  virtual ~MainWindow();
};


