#include <iostream>

#include <gtkmm/buttonbox.h>
#include <gtkmm/main.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/treestore.h>
#include <gtkmm/aboutdialog.h>
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/actiongroup.h>
#include <gtkmm/menu.h>
#include <giomm.h>

#include <gdk/gdkkeysyms-compat.h>

#include "requirements/storage/text.hpp"
#include "requirements/nodecollection.hpp"
//#include "requirements/node.hpp"
#include "requirements/select.hpp"
#include "req/console/printtree.hpp"

#include "greq/settings.hpp"
#include "greq/mainwindow.hpp"

namespace greq{

  //TODO aufräumen: rechtes Model raus
MainWindow::MainWindow()
{
  set_title("GReq");
  set_border_width(10);

  //create elements
  _topictree=Gtk::manage(new Gtk::TreeView);
  _contenttree=Gtk::manage(new Gtk::TreeView);
  _f1_button=Gtk::manage(new Gtk::Button("F1 About"));
  _f2_button=Gtk::manage(new Gtk::Button("F2 Fill dull"));
  _f3_button=Gtk::manage(new Gtk::Button("F3 Open"));
  _f10_button=Gtk::manage(new Gtk::Button("F10 Exit"));
  _recentbutton=Gtk::manage(new Gtk::MenuButton());

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
  _f3_button->signal_clicked().connect(sigc::mem_fun(*this,&MainWindow::on_f3_clicked));
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
  bottombuttonbox->add(*_f2_button);
  bottombuttonbox->add(*_f3_button);
  bottombuttonbox->add(*_f10_button);

  Gtk::Box *hbox=Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL));
  hbox->pack_start(*scrolled_left);
  hbox->pack_start(*scrolled_right);
  Gtk::Box *vbox=Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL));
  vbox->pack_start(*topbuttonbox,Gtk::PACK_SHRINK);
  vbox->pack_start(*hbox);
  vbox->pack_start(*bottombuttonbox,Gtk::PACK_SHRINK);

  //show all things
  _topictree->show();
  scrolled_left->show();
  //_contenttree->show();
  //scrolled_right->show();

  _recentbutton->show();
  topbuttonbox->show();
  _f1_button->show();
  _f2_button->show();
  _f3_button->show();
  _f10_button->show();
  bottombuttonbox->show();
  //_topictree->show();
  hbox->show();
  vbox->show();
  add(*vbox);

  Settings::getInstance().load();
  create_recent_menu();
}

MainWindow::~MainWindow(){
  //Einstellungen speichern
  Settings::getInstance().store();
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

void MainWindow::on_filename_selected(std::string filename){
  printtree(filename);
}

void MainWindow::on_f2_clicked(){
  fill_with_dull_data();
}

void MainWindow::on_f3_clicked(){
  //Select root path by dialog
  Gtk::FileChooserDialog filedlg(*this,"Select project",Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);
  filedlg.set_transient_for(*this);
  filedlg.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
  filedlg.add_button("Select", Gtk::RESPONSE_OK);
  //filedlg.show();
  int result=filedlg.run();
  switch(result){
  case Gtk::RESPONSE_OK:
    //std::cout << filedlg.get_filename() << std::endl;
    printtree(filedlg.get_filename());
    Settings::getInstance().add_project(filedlg.get_filename());
    create_recent_menu();
    break;
  case Gtk::RESPONSE_CANCEL:
    break;
  default:
    //this is for some strange behavior in terms of unknown button code
    break;
  }
}

void MainWindow::on_f10_clicked(){
  hide();
}

bool MainWindow::on_key_press(GdkEventKey *event){

  switch(event->keyval){
  case GDK_F1:
    on_f1_clicked();
    break;
  case GDK_F2:
    on_f2_clicked();
    break;
  case GDK_F3:
    on_f3_clicked();
    break;
  case GDK_F10:
    on_f10_clicked();
    break;
  default:
    break;
  }

  return true;
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

void MainWindow::printtree(std::string dirname){
  //Set root path to library
  //Auf der Kommandozeile heißt das req folder
  using namespace ::requirements;

  NodeCollection collection;
  storage::Text(collection, dirname);
  std::vector<std::string> parameters; //Hier kann noch was sinnvolles rein.
  _left_tree_model->clear();

  auto selected = requirements::select(collection, parameters, collection.getRootNode());

  for(auto& node: selected){
    add_children_to_tree(nullptr,node);
  }
  _topictree->remove_all_columns();
  _topictree->append_column("topic", _topic_columns.col_node);
  _topictree->append_column("text", _topic_columns.col_cont);

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

}
