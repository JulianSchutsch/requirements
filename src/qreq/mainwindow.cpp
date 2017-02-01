#include "qreq/mainwindow.hpp"
#include "qreq/settings.hpp"
#include "qreq/reqtextdelegate.hpp"

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

MainWindow::MainWindow(){
  generate_elements();
  generate_view();
  generate_menu();
  timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(updateTimer()));
  timer->start(1000);
}

MainWindow::~MainWindow(){
  Settings::getInstance().store();
}

void MainWindow::updateTimer() {
  std::cout<<"Timer called"<<std::endl;
  ::requirements::batch::Response modelState;
  if(_threadconnector.consumeResponse(modelState)) {
    model.consumeModel(std::move(modelState));
  }
}

void MainWindow::generate_elements(){
  Settings::getInstance().load();
  //Vorgeschlagene Standardgröße für das Hauptfenster: 800x600
  setMinimumWidth(800);
  setMinimumHeight(600);
  _reqtree=new ReqTree(this);
  _reqtree->setModel(&model);
  _reqtree->setHeaderHidden(true);
  _reqtree->setItemDelegate(new ReqTextDelegate(_reqtree));
  _reqtree->setAlternatingRowColors(false);
  connect(_reqtree, SIGNAL(expanded(QModelIndex)), this, SLOT(on_reqtree_expanded(QModelIndex)));
  connect(_reqtree, SIGNAL(collapsed(QModelIndex)), this, SLOT(on_reqtree_expanded(QModelIndex)));
  connect(_reqtree,SIGNAL(ctrl_left_pressed(QModelIndex)),this,SLOT(on_reqtree_ctrl_left(QModelIndex)));
  connect(_reqtree,SIGNAL(ctrl_right_pressed(QModelIndex)),this,SLOT(on_reqtree_ctrl_right(QModelIndex)));
  connect(_reqtree,SIGNAL(ctrl_up_pressed(QModelIndex)),this,SLOT(on_reqtree_ctrl_up(QModelIndex)));
  connect(_reqtree,SIGNAL(ctrl_down_pressed(QModelIndex)),this,SLOT(on_reqtree_ctrl_down(QModelIndex)));
  connect(_reqtree,SIGNAL(alt_return_pressed(QModelIndex)),this,SLOT(on_reqtree_alt_return(QModelIndex)));

  // TODO: Connection between reqmodel and this mainapp, needs investigation, could be directly connected to view
  //connect(_reqmodel,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(on_reqmodel_item_changed(QStandardItem*)));

  _commandline=new CommandLine(this);
  connect(_commandline,SIGNAL(fire_command(std::string)),this,SLOT(on_commandline_return(std::string)));
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
  QPushButton *f6_button=new QPushButton(tr("F6 New Sibling"));
  connect(f6_button,SIGNAL(clicked()),this,SLOT(on_f6button_clicked()));
  f6_button->setFocusPolicy(Qt::NoFocus);
  QPushButton *f7_button=new QPushButton(tr("F7 New Child"));
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
  bottombuttonbox->addWidget(f6_button);
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
}

}
