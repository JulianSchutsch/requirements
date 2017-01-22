#include "qreq/mainwindow.hpp"
#include "qreq/settings.hpp"
#include "qreq/texteditdelegate.hpp"

#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QMenu>
#include <QAction>
#include <QSignalMapper>
#include <QMenuBar>

#include <iostream>

namespace qreq{

///
/// Konstruktor.
///
MainWindow::MainWindow(){
  generate_elements();
  generate_view();
  generate_menu();
}

///
/// Destruktor.
///
MainWindow::~MainWindow(){
  Settings::getInstance().store();
}

void MainWindow::generate_elements(){
  Settings::getInstance().load();
  //Vorgeschlagene Standardgröße für das Hauptfenster: 800x600
  setMinimumWidth(800);
  setMinimumHeight(600);
  //Das ist eine Alternative zu FocusPolicy. Mal sehen, was besser ist.
  //QApplication::instance()->installEventFilter(this);
  _reqmodel=new QStandardItemModel();
  _reqmodel->setColumnCount(COLUMN_COUNT);
  //_reqtree=new QTreeView(this);
  _reqtree=new ReqTree(this);
  _reqtree->setModel(_reqmodel);
  //_reqtree->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);  //Check for Qt::ScrollBarAsNeeded
  _reqtree->setHeaderHidden(true);
  _reqtree->setItemDelegate(new TextEditDelegate(_reqtree));
  connect(_reqtree, SIGNAL(expanded(QModelIndex)), this, SLOT(on_reqtree_expanded(QModelIndex)));
  connect(_reqtree, SIGNAL(collapsed(QModelIndex)), this, SLOT(on_reqtree_expanded(QModelIndex)));
  connect(_reqtree,SIGNAL(ctrl_left_pressed(QModelIndex)),this,SLOT(on_reqtree_ctrl_left(QModelIndex)));
  connect(_reqtree,SIGNAL(ctrl_right_pressed(QModelIndex)),this,SLOT(on_reqtree_ctrl_right(QModelIndex)));
  connect(_reqtree,SIGNAL(ctrl_up_pressed(QModelIndex)),this,SLOT(on_reqtree_ctrl_up(QModelIndex)));
  connect(_reqtree,SIGNAL(ctrl_down_pressed(QModelIndex)),this,SLOT(on_reqtree_ctrl_down(QModelIndex)));
  connect(_reqtree,SIGNAL(alt_return_pressed(QModelIndex)),this,SLOT(on_reqtree_alt_return(QModelIndex)));
  connect(_reqmodel,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(on_reqmodel_item_changed(QStandardItem*)));

  _commandline=new CommandLine(this);
  connect(_commandline,SIGNAL(fire_command(std::string)),this,SLOT(on_commandline_return(std::string)));
  _threadconnector.init();
}

void MainWindow::generate_view(){
  QPushButton *f1_button=new QPushButton(tr("F1 Keycodes"));
  connect(f1_button,SIGNAL(clicked()),this,SLOT(on_f1button_clicked()));
  f1_button->setFocusPolicy(Qt::NoFocus);
  QPushButton *f2_button=new QPushButton(tr("F2 Save"));
  connect(f2_button,SIGNAL(clicked()),this,SLOT(on_f2button_clicked()));
  f2_button->setFocusPolicy(Qt::NoFocus);
  QPushButton *f3_button=new QPushButton(tr("F3 Open"));
  connect(f3_button,SIGNAL(clicked()),this,SLOT(on_f3button_clicked()));
  f3_button->setFocusPolicy(Qt::NoFocus);
  QPushButton *f4_button=new QPushButton(tr("F4 Edit"));
  connect(f4_button,SIGNAL(clicked()),this,SLOT(on_f4button_clicked()));
  f4_button->setFocusPolicy(Qt::NoFocus);
  QPushButton *f5_button=new QPushButton(tr("F5 Copy"));
  connect(f5_button,SIGNAL(clicked()),this,SLOT(on_f5button_clicked()));
  f5_button->setFocusPolicy(Qt::NoFocus);
  QPushButton *f7_button=new QPushButton(tr("F7 New"));
  connect(f7_button,SIGNAL(clicked()),this,SLOT(on_f7button_clicked()));
  f7_button->setFocusPolicy(Qt::NoFocus);
  QPushButton *f8_button=new QPushButton(tr("F8 Delete"));
  connect(f8_button,SIGNAL(clicked()),this,SLOT(on_f8button_clicked()));
  f8_button->setFocusPolicy(Qt::NoFocus);
  QPushButton *f9_button=new QPushButton(tr("F9 About"));
  connect(f9_button,SIGNAL(clicked()),this,SLOT(on_f9button_clicked()));
  f9_button->setFocusPolicy(Qt::NoFocus);
  QPushButton *f10_button=new QPushButton(tr("F10 Exit"));
  connect(f10_button,SIGNAL(clicked()),this,SLOT(on_f10button_clicked()));
  f10_button->setFocusPolicy(Qt::NoFocus);

  QHBoxLayout *bottombuttonbox=new QHBoxLayout();
  bottombuttonbox->addWidget(f1_button);
  bottombuttonbox->addWidget(f2_button);
  bottombuttonbox->addWidget(f3_button);
  bottombuttonbox->addWidget(f4_button);
  bottombuttonbox->addWidget(f5_button);
  bottombuttonbox->addWidget(f7_button);
  bottombuttonbox->addWidget(f8_button);
  bottombuttonbox->addWidget(f9_button);
  bottombuttonbox->addWidget(f10_button);

  QPushButton *newblob_button=new QPushButton(tr("New Blob"));
  connect(newblob_button,SIGNAL(clicked()),this,SLOT(on_newblobbutton_clicked()));
  newblob_button->setFocusPolicy(Qt::NoFocus);
  QPushButton *linkblob_button=new QPushButton(tr("Link Blob"));
  connect(linkblob_button,SIGNAL(clicked()),this,SLOT(on_linkblobbutton_clicked()));
  linkblob_button->setFocusPolicy(Qt::NoFocus);

  QVBoxLayout *leftbuttonbox=new QVBoxLayout();
  leftbuttonbox->addWidget(newblob_button);
  leftbuttonbox->addWidget(linkblob_button);
  leftbuttonbox->addStretch();

  QVBoxLayout *rightbox=new QVBoxLayout();
  rightbox->addWidget(_reqtree);
  rightbox->addWidget(_commandline);
  QHBoxLayout *hbox=new QHBoxLayout();
  hbox->addLayout(leftbuttonbox);
  hbox->addLayout(rightbox);

  QVBoxLayout *vbox=new QVBoxLayout();
  vbox->addLayout(hbox);
  vbox->addLayout(bottombuttonbox);
  QWidget *mainwidget=new QWidget();
  mainwidget->setLayout(vbox);

  setCentralWidget(mainwidget);
}

///
/// Erzeugt das Menu.
///
void MainWindow::generate_menu(){
  generate_menu_recent();
}

void MainWindow::generate_menu_recent(){
  //Menu erzeugen
  QMenu *recentmenu=new QMenu("Recent");
  QMenuBar *menubar=new QMenuBar();
  QSignalMapper* signalMapper = new QSignalMapper (this) ;
  for(auto filename : Settings::getInstance().last_projects()){
    std::cout << "adding " << filename << std::endl;
    QAction *openact=new QAction(filename.c_str(),this);
    connect (openact, SIGNAL(triggered()), signalMapper, SLOT(map())) ;
    signalMapper -> setMapping (openact, filename.c_str()) ;
    recentmenu->addAction(openact);
  }
  connect (signalMapper, SIGNAL(mapped(QString)), this, SLOT(on_openact_triggered(QString const&)));
  menubar->addMenu(recentmenu);
  setMenuBar(menubar);
  //Und jetzt gleich das letzte Projekt laden
  if(Settings::getInstance().current_project!="") load_current_project();
}

void MainWindow::set_current_project(std::string const& filename){
  Settings::getInstance().current_project=filename;
  load_current_project();
}

void MainWindow::load_current_project(){
  init_project();
  printtree();
}

void MainWindow::set_focus_to_uuid(QStandardItem *parent_item, std::string const& uuid){
  for(int i=0;i<parent_item->rowCount();i++){
    QStandardItem* item_id=parent_item->child(i,COLUMN_ID);
    QStandardItem* item_text=parent_item->child(i,COLUMN_TEXT);
    if(item_id->text().toStdString()==uuid){
      //Gefunden, also hinspringen
      //_reqtree->scrollTo(item_text->index());
      _reqtree->setCurrentIndex(item_text->index());
      /*QItemSeletionModel *selection=_reqtree->selectionModel();
      selection->select (QItemSelection (
        treeview->model ()->index (search, 0),
        treeview->model ()->index (search, treeview->model ()->columnCount () - 1)),
          QItemSelectionModel::Select);*/
    }
    else{
      //War nicht dabei, vielleicht in den Kindern?
      set_focus_to_uuid(item_text,uuid);
    }
  }
}

std::string MainWindow::get_uuid_by_modelindex(const QModelIndex& index){
  std::string retval="";
  QStandardItem* item_parent=get_parent_item_by_modelindex(index);
  if(item_parent!=nullptr){
    QStandardItem* item_id=item_parent->child(index.row(),COLUMN_ID);
    if(item_id!=nullptr){
      retval=item_id->text().toStdString();
    }
  }
  return retval;
}

std::string MainWindow::get_text_by_modelindex(const QModelIndex& index){
  std::string retval="";
  QStandardItem* item_parent=get_parent_item_by_modelindex(index);
  if(item_parent!=nullptr){
    QStandardItem* item_text=item_parent->child(index.row(),COLUMN_TEXT);
    if(item_text!=nullptr){
      retval=item_text->text().toStdString();
    }
  }
  return retval;
}

QStandardItem* MainWindow::get_parent_item_by_modelindex(const QModelIndex& index){
  QStandardItem* retval=nullptr;
  QStandardItem* item=_reqmodel->itemFromIndex(index);
  if(item!=nullptr){
    retval=item->parent();
    if(retval==nullptr){
      //vielleicht ist der parent ja das invisibleRootItem?
      //Versuchen wirs mal
      retval=_reqmodel->invisibleRootItem();
    }
  }
  return retval;
}

void MainWindow::add_blob_to_row(QModelIndex const& index,std::string const& blobtext){
//void MainWindow::add_blob_to_row(Gtk::TreeModel::iterator selected_row,std::string const& blobtext){
  if(blobtext!=""){
    QStandardItem* item_parent=get_parent_item_by_modelindex(index);
    if(item_parent!=nullptr){
      QStandardItem* item_text=item_parent->child(index.row(),COLUMN_TEXT);
      if(item_text!=nullptr){
        std::string content=item_text->text().toStdString();
        content+="\n%(blob:";
        content+=blobtext;
        content+=")\n";
        //In den Baum schreiben
        item_text->setText(content.c_str());
        //Jetzt in die collection committen
        commit_to_collection(get_uuid_by_modelindex(index),content);
      }
    }


    //Gtk::TreeModel::Row row = *selected_row;
    //Glib::ustring content = row[_topic_columns.col_cont];
    //content+="\n%(blob:";
    //content+=blobtext;
    //content+=")\n";
    ////In den Baum schreiben, das changed()-Signal sorgt dafür, dass das in die collection kommt
    //row[_topic_columns.col_cont]=content;
  }
}

}
