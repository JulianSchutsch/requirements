#include <iostream>

#include <gtkmm/buttonbox.h>
#include <gtkmm/main.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/treestore.h>
#include <gtkmm/aboutdialog.h>

#include <gdk/gdkkeysyms-compat.h>

#include "greq/mainwindow.hpp"

MainWindow::MainWindow()
{
  set_title("GReq");
  set_border_width(10);

  //create elements
  _topictree=Gtk::manage(new Gtk::TreeView);
  _contenttree=Gtk::manage(new Gtk::TreeView);
  _f1_button=Gtk::manage(new Gtk::Button("F1 About"));
  _f2_button=Gtk::manage(new Gtk::Button("F2 Fill Dull"));
  _f10_button=Gtk::manage(new Gtk::Button("F10 Exit"));

  //create left treemodel
  _left_tree_model = Gtk::TreeStore::create(_topic_columns);
  _topictree->set_model(_left_tree_model);
  _topictree->set_reorderable();

  //create right listmodel
  _right_list_model=Gtk::ListStore::create(_content_columns);
  _contenttree->set_model(_right_list_model);

  //connect signals
  _f1_button->signal_clicked().connect(sigc::mem_fun(*this,&MainWindow::on_f1_clicked));
  _f2_button->signal_clicked().connect(sigc::mem_fun(*this,&MainWindow::on_f2_clicked));
  _f10_button->signal_clicked().connect(sigc::mem_fun(*this,&MainWindow::on_f10_clicked));
  this->signal_key_press_event().connect(sigc::mem_fun(*this, &MainWindow::on_key_press));

  //pack all things
  //Left TreeView
  Gtk::ScrolledWindow* scrolled_left=Gtk::manage(new Gtk::ScrolledWindow);
  scrolled_left->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  scrolled_left->add(*_topictree);
  //Right TreeView
  Gtk::ScrolledWindow* scrolled_right=Gtk::manage(new Gtk::ScrolledWindow);
  scrolled_right->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  scrolled_right->add(*_contenttree);

  //Buttons
  Gtk::ButtonBox *bottombuttonbox=Gtk::manage(new Gtk::ButtonBox(Gtk::ORIENTATION_HORIZONTAL));
  bottombuttonbox->set_border_width(5);
  bottombuttonbox->set_spacing(Gtk::PACK_EXPAND_WIDGET);
  bottombuttonbox->set_layout(Gtk::BUTTONBOX_SPREAD);
  bottombuttonbox->add(*_f1_button);
  bottombuttonbox->add(*_f2_button);
  bottombuttonbox->add(*_f10_button);

  Gtk::Box *hbox=Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL));
  hbox->pack_start(*scrolled_left);
  hbox->pack_start(*scrolled_right);
  Gtk::Box *vbox=Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL));
  vbox->pack_start(*hbox);
  vbox->pack_start(*bottombuttonbox,Gtk::PACK_SHRINK);

  //show all things
  _topictree->show();
  scrolled_left->show();
  _contenttree->show();
  scrolled_right->show();

  _f1_button->show();
  _f2_button->show();
  _f10_button->show();
  bottombuttonbox->show();
  _topictree->show();
  hbox->show();
  vbox->show();
  add(*vbox);
}

MainWindow::~MainWindow(){
}

void MainWindow::on_f1_clicked()
{
  Gtk::AboutDialog about;
  about.set_transient_for(*this);
  about.set_program_name("Über");
  about.set_version("0.0.0");
  about.set_website("https://github.com/JulianSchutsch/requirements");
  std::vector<Glib::ustring> list_authors;
  list_authors.push_back("Julian Schutsch");
  list_authors.push_back("Dirk Neumann");
  about.set_authors(list_authors);
  
  about.show();
  about.run();
}

void MainWindow::on_f2_clicked()
{
  fill_with_dull_data();

}

void MainWindow::on_f10_clicked()
{
  hide();
}

bool MainWindow::on_key_press(GdkEventKey *event){

  if (event->keyval == GDK_F10) on_f10_clicked();
  else if (event->keyval == GDK_F2) on_f2_clicked();

  return true;
}

void MainWindow::fill_with_dull_data(){
  //Left
  //Fill the TreeView's model
  //clear Tree Model
  _left_tree_model->clear();
  Gtk::TreeModel::Row row = *(_left_tree_model->append());
  row[_topic_columns.col_node] = "1";

  Gtk::TreeModel::Row childrow = *(_left_tree_model->append(row.children()));
  childrow[_topic_columns.col_node] = "11";

  childrow = *(_left_tree_model->append(row.children()));
  childrow[_topic_columns.col_node] = "12";

  row = *(_left_tree_model->append());
  row[_topic_columns.col_node] = "2";

  row = *(_left_tree_model->append());
  row[_topic_columns.col_node] = "3";

  childrow = *(_left_tree_model->append(row.children()));
  childrow[_topic_columns.col_node] = "31";

  //Add the TreeView's view columns:
  _topictree->remove_all_columns();
  _topictree->append_column("topic", _topic_columns.col_node);

  //Right
  //Fill the TreeView's model
  _right_list_model->clear();
  Gtk::TreeModel::Row rrow = *(_right_list_model->append());
  rrow[_content_columns.col_id] = 1;
  rrow[_content_columns.col_text] = "Erster Text";

  rrow = *(_right_list_model->append());
  rrow[_content_columns.col_id] = 2;
  rrow[_content_columns.col_text] = "Zweiter Text";

  rrow = *(_right_list_model->append());
  rrow[_content_columns.col_id] = 3;
  rrow[_content_columns.col_text] = "Dritter Text";

  //Add the TreeView's view columns:
  _contenttree->remove_all_columns();
  //This number will be shown with the default numeric formatting.
  _contenttree->append_column("ID", _content_columns.col_id);
  _contenttree->append_column("Text", _content_columns.col_text);
}
