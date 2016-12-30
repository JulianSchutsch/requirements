#include <iostream>

#include <gtkmm/buttonbox.h>
#include <gtkmm/main.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/treestore.h>
#include <gtkmm/actiongroup.h>
#include <gtkmm/menu.h>
#include <giomm.h>

#include "greq/settings.hpp"
#include "greq/mainwindow.hpp"

namespace greq{
//TODO: Feature:  1. Hinzufügen von Knoten
//                2. Löschen von Knoten
//                3. Verschieben eines Knotens an eine andere Stelle im Baum
//                   shift+links = Hierarchieebene hoch
//                   shift+rechts = Hierarchieebene runter
//                   shift+hoch = In gleicher Ebene nach vorn
//                   shift+runter = In gleicher Ebene runter
//                4. Bessere Darstellung der Knoten
MainWindow::MainWindow()
{
  set_title("GReq");
  set_border_width(10);
  set_default_size(800,600);

  _changed_signal_ignore=0;
  //create elements
  _topictree=Gtk::manage(new Gtk::TreeView);
  _f1_button=Gtk::manage(new Gtk::Button("F1 About"));
  _f2_button=Gtk::manage(new Gtk::Button("F2 Save"));
  _f3_button=Gtk::manage(new Gtk::Button("F3 Open"));
  _f4_button=Gtk::manage(new Gtk::Button("F4 Edit"));
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
  _f2_button->signal_clicked().connect(sigc::mem_fun(*this,&MainWindow::on_f2_clicked));
  _f3_button->signal_clicked().connect(sigc::mem_fun(*this,&MainWindow::on_f3_clicked));
  _f4_button->signal_clicked().connect(sigc::mem_fun(*this,&MainWindow::on_f4_clicked));
  _f5_button->signal_clicked().connect(sigc::mem_fun(*this,&MainWindow::on_f5_clicked));
  _f6_button->signal_clicked().connect(sigc::mem_fun(*this,&MainWindow::on_f6_clicked));
  _f7_button->signal_clicked().connect(sigc::mem_fun(*this,&MainWindow::on_f7_clicked));
  _f8_button->signal_clicked().connect(sigc::mem_fun(*this,&MainWindow::on_f8_clicked));
  _f10_button->signal_clicked().connect(sigc::mem_fun(*this,&MainWindow::on_f10_clicked));
  this->signal_key_press_event().connect(sigc::mem_fun(*this, &MainWindow::on_key_press),false);
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
  bottombuttonbox->add(*_f2_button);
  bottombuttonbox->add(*_f3_button);
  bottombuttonbox->add(*_f4_button);
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
  _f2_button->show();
  _f3_button->show();
  _f4_button->show();
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

void MainWindow::remove_children_from_tree(Gtk::TreeModel::Row* row){
  //remove all children of row.
  Gtk::TreeNodeChildren children=row->children();
  remove_children_from_tree(children);
}

void MainWindow::remove_children_from_tree(Gtk::TreeNodeChildren children){
  for(auto child:children){
    if(child->children().size()>0){
      //Noch Enkel da, Enkel löschen
      remove_children_from_tree(child->children());
    }
  }
  //Alle Enkel gelöscht, jetzt Kinder löschen
  Gtk::TreeNodeChildren::iterator iter=children.begin();
  ++_changed_signal_ignore;
  while(iter!=children.end()) iter=_left_tree_model->erase(iter);
  --_changed_signal_ignore;

}

void MainWindow::add_child_to_tree(Gtk::TreeModel::Row* row,const requirements::NodePtr& node){
  ++_changed_signal_ignore;
  Gtk::TreeModel::Row childrow;
  if(row!=nullptr) childrow = *(_left_tree_model->append(row->children()));
  else childrow=*(_left_tree_model->append());
  childrow[_topic_columns.col_node] = requirements::id_to_string(node->getId());
  childrow[_topic_columns.col_cont] = node->getContent();
  add_children_to_tree(&childrow,node);
  --_changed_signal_ignore;
}

void MainWindow::add_children_to_tree(Gtk::TreeModel::Row* row,const requirements::NodePtr& node){
  auto& children=node->getChildren();
  for(auto& elem: children){
    add_child_to_tree(row,elem);
  }
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

void MainWindow::set_current_project(std::string const& filename){
  Settings::getInstance().current_project=filename;
  init_collection();
  printtree();
}

void MainWindow::set_focus_to_uuid(Gtk::TreeModel::Row* parent, std::string const& uuid){
  //Glib::ustring newuuid=(*parent)[_topic_columns.col_node];
  //std::cout << "parent_uuid: "<< newuuid << std::endl;
  Gtk::TreeNodeChildren children=parent->children();
  //So, eins der Children hat die passende uuid
  for(auto& elem: children){
    Glib::ustring model_node = (*elem)[_topic_columns.col_node];
    if(model_node==(Glib::ustring)uuid){
      //Found, now set focus
      Gtk::TreeModel::Path path=_topictree->get_model()->get_path(*elem);
      _topictree->set_cursor(path);
    }
  }
}

void MainWindow::set_focus_to_uuid(std::string const& uuid){
  Gtk::TreeModel::Children children=_left_tree_model->children();
  set_focus_to_uuid(children,uuid);

}

void MainWindow::set_focus_to_uuid(Gtk::TreeModel::Children const& children,std::string const& uuid){
  for(auto child:children){
    if(child->children().size()>0){
      //weiter absteigen
      set_focus_to_uuid(child->children(),uuid);
    }
    Gtk::TreeModel::Row row=*child;
    Glib::ustring read_uuid=row[_topic_columns.col_node];
    if(read_uuid==uuid){
      //Ha, gefunden.
      Gtk::TreeModel::Path path=_left_tree_model->get_path(*child);
      _topictree->set_cursor(path);
    }
    //std::cout << "uuid: " << uuid << std::endl;
  }
}


// uuid_of_focus_node gibt den Knoten an, der den Focus bekommen soll, z.B. nach einer Verschiebung
void MainWindow::reprint_tree_below_parent_of(Gtk::TreeModel::Row* row,std::string const& uuid_of_focus_node){
  Gtk::TreeModel::Path path=_topictree->get_model()->get_path(*row);
  Glib::ustring uuid=(*row)[_topic_columns.col_node];
  requirements::NodePtr node=get_node_for_uuid(uuid);
  if(path.up()==true){  //Häh? warum ist denn path.up() immer true, auch wenns kein parent gibt?
    //Wir haben einen Parent im Tree gefunden
    //Jetzt muss ich erst mal das row zum path finden.
    Gtk::TreeModel::iterator iter1 = _topictree->get_model()->get_iter(path);
    Gtk::TreeModel::Row parent_row = *iter1;

    remove_children_from_tree(&parent_row);
    //Jetzt suchen wir uns den node-Parent
    //requirements::NodePtr parent_node=node->getParent();
    ++_changed_signal_ignore;
    add_children_to_tree(&parent_row,node->getParent());
    --_changed_signal_ignore;

    //Jetzt noch wieder an der Stelle parent_row aufklappen
    _topictree->expand_row(path,false);
    //Und jetzt an die Stelle des neuen Knotens springen
    if(uuid_of_focus_node!="") set_focus_to_uuid(&parent_row,uuid_of_focus_node);
  }
  else{
    //Es gibt keinen Vorgängerknoten.
    //Einen zusätzlichen neuen Knoten dranhängen.
    //Aus irgend einem Grund kommen wir hier gar nicht rein.
    add_children_to_tree(nullptr,node->getParent());
  }
}

std::string MainWindow::get_uuid_on_cursor(){
  std::string retval="";
  Glib::RefPtr<Gtk::TreeSelection> selection = _topictree->get_selection();
  Gtk::TreeModel::iterator selected_row = selection->get_selected();
  if(selected_row!=nullptr){
    Gtk::TreeModel::Row row = *selected_row;
    Glib::ustring uuid = row[_topic_columns.col_node];
    retval=uuid;
    //requirements::NodePtr node=get_node_for_uuid(uuid);
  }
  return retval;
}

}
