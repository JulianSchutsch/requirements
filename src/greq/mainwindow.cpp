#include <iostream>

#include <gtkmm/buttonbox.h>
#include <gtkmm/main.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/treestore.h>
#include <gtkmm/actiongroup.h>
#include <gtkmm/menu.h>
#include <giomm.h>


#include "requirements/storage/text.hpp"
#include "requirements/nodecollection.hpp"
#include "requirements/select.hpp"


#include "greq/settings.hpp"
#include "greq/mainwindow.hpp"

namespace greq{
//TODO: Feature:  1. Hinzufügen von Knoten
//                2. Löschen von Knoten
//                3. Verschieben eines Knotens an eine andere Stelle im Baum
//                4. Bessere Darstellung der Knoten
MainWindow::MainWindow()
{
  set_title("GReq");
  set_border_width(10);
  set_default_size(800,600);

  _changed_signal_ignore=false;
  //create elements
  _topictree=Gtk::manage(new Gtk::TreeView);
  _f1_button=Gtk::manage(new Gtk::Button("F1 About"));
  _f3_button=Gtk::manage(new Gtk::Button("F3 Open"));
  _f5_button=Gtk::manage(new Gtk::Button("F5 Copy"));
  _f6_button=Gtk::manage(new Gtk::Button("F6 Move"));
  _f7_button=Gtk::manage(new Gtk::Button("F7 New"));
  _f8_button=Gtk::manage(new Gtk::Button("F8 Delete"));
  _f10_button=Gtk::manage(new Gtk::Button("F10 Exit"));
  _recentbutton=Gtk::manage(new Gtk::MenuButton());

  //create left treemodel
  _left_tree_model = Gtk::TreeStore::create(_topic_columns);
  _topictree->set_model(_left_tree_model);
  _topictree->set_reorderable();


  //connect signals
  _f1_button->signal_clicked().connect(sigc::mem_fun(*this,&MainWindow::on_f1_clicked));
  _f3_button->signal_clicked().connect(sigc::mem_fun(*this,&MainWindow::on_f3_clicked));
  _f5_button->signal_clicked().connect(sigc::mem_fun(*this,&MainWindow::on_f5_clicked));
  _f6_button->signal_clicked().connect(sigc::mem_fun(*this,&MainWindow::on_f6_clicked));
  _f7_button->signal_clicked().connect(sigc::mem_fun(*this,&MainWindow::on_f7_clicked));
  _f8_button->signal_clicked().connect(sigc::mem_fun(*this,&MainWindow::on_f8_clicked));
  _f10_button->signal_clicked().connect(sigc::mem_fun(*this,&MainWindow::on_f10_clicked));
  this->signal_key_press_event().connect(sigc::mem_fun(*this, &MainWindow::on_key_press));
  _left_tree_model->signal_row_changed().connect(sigc::mem_fun(this,&MainWindow::on_topic_row_changed));

  //pack all things
  //Left TreeView
  Gtk::ScrolledWindow* scrolled_left=Gtk::manage(new Gtk::ScrolledWindow);
  scrolled_left->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  scrolled_left->add(*_topictree);

  //Buttons
  Gtk::ButtonBox *topbuttonbox=Gtk::manage(new Gtk::ButtonBox(Gtk::ORIENTATION_HORIZONTAL));
  topbuttonbox->set_border_width(5);
  topbuttonbox->set_spacing(Gtk::PACK_EXPAND_WIDGET);
  topbuttonbox->set_layout(Gtk::BUTTONBOX_START);
  topbuttonbox->add(*_recentbutton);

  Gtk::ButtonBox *bottombuttonbox=Gtk::manage(new Gtk::ButtonBox(Gtk::ORIENTATION_HORIZONTAL));
  bottombuttonbox->set_border_width(5);
  bottombuttonbox->set_spacing(Gtk::PACK_EXPAND_WIDGET);
  bottombuttonbox->set_layout(Gtk::BUTTONBOX_SPREAD);
  bottombuttonbox->add(*_f1_button);
  bottombuttonbox->add(*_f3_button);
  bottombuttonbox->add(*_f5_button);
  bottombuttonbox->add(*_f6_button);
  bottombuttonbox->add(*_f7_button);
  bottombuttonbox->add(*_f8_button);
  bottombuttonbox->add(*_f10_button);

  //Gtk::Box *hbox=Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL));
  Gtk::Box *vbox=Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL));
  vbox->pack_start(*topbuttonbox,Gtk::PACK_SHRINK);
  vbox->pack_start(*scrolled_left);
  vbox->pack_start(*bottombuttonbox,Gtk::PACK_SHRINK);

  //show all things
  _topictree->show();
  scrolled_left->show();

  _recentbutton->show();
  topbuttonbox->show();
  _f1_button->show();
  _f3_button->show();
  _f5_button->show();
  _f6_button->show();
  _f7_button->show();
  _f8_button->show();
  _f10_button->show();
  bottombuttonbox->show();
  //hbox->show();
  vbox->show();
  add(*vbox);

  Settings::getInstance().load();
  create_recent_menu();
}

MainWindow::~MainWindow(){
  Settings::getInstance().store();
}

void MainWindow::add_child_to_tree(Gtk::TreeModel::Row* row,const requirements::NodePtr& node){
  Gtk::TreeModel::Row childrow;
  if(row!=nullptr) childrow = *(_left_tree_model->append(row->children()));
  else childrow=*(_left_tree_model->append());
  childrow[_topic_columns.col_node] = requirements::id_to_string(node->getId());
  childrow[_topic_columns.col_cont] = node->getContent();
  add_children_to_tree(&childrow,node);

}

void MainWindow::add_children_to_tree(Gtk::TreeModel::Row* row,const requirements::NodePtr& node){
  auto& children=node->getChildren();
  for(auto& elem: children) add_child_to_tree(row,elem);
}

void MainWindow::create_recent_menu(){
  //destroy old menu
  Gtk::Menu *oldmenu=_recentbutton->get_popup();
  if(oldmenu!=nullptr){
    //destroy old menu
    delete(oldmenu);
  }
  Gtk::Menu *recentmenu = Gtk::manage(new Gtk::Menu());
  auto refActionGroup = Gio::SimpleActionGroup::create();
  for (auto filename : Settings::getInstance().last_projects()){
    //std::cout << filename << std::endl;
    Gtk::MenuItem *menuitem_file = Gtk::manage(new Gtk::MenuItem(filename, true));
    menuitem_file->signal_activate().connect(sigc::bind<std::string>(sigc::mem_fun(*this,&MainWindow::on_filename_selected),filename));
    menuitem_file->show();
    recentmenu->append(*menuitem_file);
  }
  _recentbutton->set_popup(*recentmenu);
}

void MainWindow::on_topic_row_changed(const Gtk::TreeModel::Path& path, const Gtk::TreeModel::iterator& iter){
  (void)path;
  if(_changed_signal_ignore==false){
    if(iter){
      //Dann muss es jetzt committed werden
      Gtk::TreeModel::Row row = *iter;
      Glib::ustring model_value = row[_topic_columns.col_cont];
      Glib::ustring model_node = row[_topic_columns.col_node];

      commit_to_collection(model_node,model_value);
    }
  }
}

void MainWindow::set_current_project(std::string const& filename){
  Settings::getInstance().current_project=filename;
  init_collection();
  printtree();
}

}
