#include "qreq/mainwindow.hpp"
#include "qreq/settings.hpp"
#include "qreq/reqtextdelegate.hpp"

#include "requirements/commands/folder.hpp"

#include "util/formatstring.hpp"

#include <QApplication>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QMenu>
#include <QAction>
#include <QSignalMapper>
#include <QMenuBar>
#include <QShortcut>

#include <iostream>
#include <functional>

namespace qreq {

  void MainWindow::action_quit() {
    QApplication::exit(0);
  }

  MainWindow::MainWindow()
    : manipulator(model) {
    generate_elements();
    generate_view();
    generate_menu();
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTimer()));
    timer->start(1000);
    {
      QShortcut* shortcut = new QShortcut(QKeySequence("F10"), this);
      connect(shortcut, SIGNAL(activated()), this, SLOT(action_quit()));
    }
  }

  MainWindow::~MainWindow() {
    //Copy Last commands to Settings
    strvec last_commands=_commandline->getLastCommands(20);  //TODO Diese Zahl könnte man konfigurierbar machen
    Settings::getInstance().last_commands(last_commands);
    //Store Settings
    Settings::getInstance().store();
  }

  void MainWindow::printMessage(const BatchMessage &message) {
    auto item = new QListWidgetItem(util::formatString(message.message, message.parameters).c_str());
    switch(message.kind) {
      case ::requirements::Status::MessageKind::InternalError:
      case ::requirements::Status::MessageKind::UserError:
      case ::requirements::Status::MessageKind::OtherError:
        item->setForeground(Qt::red);
        break;
      default:
        break;
    }
    _messageList->addItem(item);
    _messageList->scrollToBottom();
  }

  void MainWindow::updateTimer() {
    model.checkResponses();
    auto newMessages = model.consumeMessages();
    for (auto &message:newMessages) {
      printMessage(message);
    }
  }

  void MainWindow::generate_elements() {
    Settings::getInstance().load();
    //Vorgeschlagene Standardgröße für das Hauptfenster: 800x600
    setMinimumWidth(800);
    setMinimumHeight(600);
    _reqtree = new ReqTree(this);
    _reqtree->setHeaderHidden(true);
    _reqtree->setItemDelegate(new ReqTextDelegate(_reqtree));
    _reqtree->setAlternatingRowColors(false);
    _reqtree->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    _reqtree->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    _reqtree->setModel(&model);
    _reqtree->setUniformRowHeights(false);
    _reqtree->header()->setStretchLastSection(false);
    _reqtree->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    _reqtree->setAutoScroll(true);
    connect(_reqtree, SIGNAL(expanded(QModelIndex)), this, SLOT(on_reqtree_expanded(QModelIndex)));
    connect(_reqtree, SIGNAL(collapsed(QModelIndex)), this, SLOT(on_reqtree_expanded(QModelIndex)));
    connect(_reqtree, SIGNAL(ctrl_left_pressed(QModelIndex)), this, SLOT(on_reqtree_ctrl_left(QModelIndex)));
    connect(_reqtree, SIGNAL(ctrl_right_pressed(QModelIndex)), this, SLOT(on_reqtree_ctrl_right(QModelIndex)));
    connect(_reqtree, SIGNAL(ctrl_up_pressed(QModelIndex)), this, SLOT(on_reqtree_ctrl_up(QModelIndex)));
    connect(_reqtree, SIGNAL(ctrl_down_pressed(QModelIndex)), this, SLOT(on_reqtree_ctrl_down(QModelIndex)));
    connect(_reqtree, SIGNAL(alt_return_pressed(QModelIndex)), this, SLOT(on_reqtree_alt_return(QModelIndex)));
    connect(_reqtree,SIGNAL(ctrl_alt_return_pressed(QModelIndex)),this,SLOT(on_reqtree_ctrl_alt_return(QModelIndex)));
    connect(&model,SIGNAL(modelReset()),this,SLOT(on_model_reset()));

    _commandline = new CommandLine(this,Settings::getInstance().last_commands());
    connect(_commandline, SIGNAL(fire_command(std::string)), this, SLOT(on_commandline_return(std::string)));

    _messageList = new QListWidget(this);
    _messageList->setMaximumHeight(100);
    auto sizePolicy = _messageList->sizePolicy();
    sizePolicy.setVerticalPolicy(QSizePolicy::Policy::Fixed);
    _messageList->setSizePolicy(sizePolicy);
    connect(&manipulator,SIGNAL(change_to_viewpos(QModelIndex)),this,SLOT(on_manipulate_viewpos(QModelIndex)));
  }

  void MainWindow::generate_view() {

    //Default-Tasten
    std::array<std::string,10> keynames={"F1","F2","F3","F4","F5","F6","F7","F8","F9","F10"};
    std::unordered_map<std::string,QString> fkey_texts={
        {keynames[0],tr("F1 Keycodes")},
        {keynames[1],tr("F2 Edit")},
        {keynames[2],tr("F3 Open")},
        {keynames[3],tr("F4 No Function")},
        {keynames[4],tr("F5 Copy")},
        {keynames[5],tr("F6 New Sibling")},
        {keynames[6],tr("F7 New Child")},
        {keynames[7],tr("F8 Delete")},
        {keynames[8],tr("F9 About")},
        {keynames[9],tr("F10 Exit")}
    };
    //Tasten-Handler
    std::unordered_map<std::string,std::function<void()> > fkey_handler={
        {keynames[0],std::bind(&MainWindow::on_f1button_clicked,this)},
        {keynames[1],std::bind(&MainWindow::on_f2button_clicked,this)},
        {keynames[2],std::bind(&MainWindow::on_f3button_clicked,this)},
        {keynames[3],std::bind(&MainWindow::on_f4button_clicked,this)},
        {keynames[4],std::bind(&MainWindow::on_f5button_clicked,this)},
        {keynames[5],std::bind(&MainWindow::on_f6button_clicked,this)},
        {keynames[6],std::bind(&MainWindow::on_f7button_clicked,this)},
        {keynames[7],std::bind(&MainWindow::on_f8button_clicked,this)},
        {keynames[8],std::bind(&MainWindow::on_f9button_clicked,this)},
        {keynames[9],std::bind(&MainWindow::on_f10button_clicked,this)}
    };
    //Button-Map
    std::map<std::string,QPushButton*> buttonmap;
    //Buttonbox fürs Layout
    QHBoxLayout *bottombuttonbox = new QHBoxLayout();
    for(auto& keyname:keynames){
      buttonmap[keyname]=new QPushButton(fkey_texts[keyname]);
      //New Qt-Signal-Slot-Syntax
      connect(buttonmap[keyname],&QPushButton::clicked,fkey_handler[keyname]);
      buttonmap[keyname]->setFocusPolicy(Qt::NoFocus);
      bottombuttonbox->addWidget(buttonmap[keyname]);
    }

    QVBoxLayout *rightbox = new QVBoxLayout();
    rightbox->addWidget(_reqtree);
    rightbox->addWidget(_messageList);
    rightbox->addWidget(_commandline);

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addLayout(rightbox);
    vbox->addLayout(bottombuttonbox);
    QWidget *mainwidget = new QWidget();
    mainwidget->setLayout(vbox);

    setCentralWidget(mainwidget);
  }

///
/// Erzeugt das Menu.
///
  void MainWindow::generate_menu() {
    generate_menu_recent();
  }

  void MainWindow::generate_menu_recent() {
    //Menu erzeugen
    QMenu *recentmenu = new QMenu("Recent");
    QMenuBar *menubar = new QMenuBar();
    QSignalMapper *signalMapper = new QSignalMapper(this);
    for (auto filename : Settings::getInstance().last_projects()) {
      QAction *openact = new QAction(filename.c_str(), this);
      connect(openact, SIGNAL(triggered()), signalMapper, SLOT(map()));
      signalMapper->setMapping(openact, filename.c_str());
      recentmenu->addAction(openact);
    }
    connect(signalMapper, SIGNAL(mapped(QString)), this, SLOT(on_openact_triggered(QString const&)));
    menubar->addMenu(recentmenu);
    setMenuBar(menubar);
  }

  void MainWindow::set_current_project(std::string const &filename) {
    Settings::getInstance().add_project(filename);
    load_current_project();
  }

  void MainWindow::load_current_project() {
    //Schade, dass ich draußen bleiben muss. Naja, schreib ich halt nen Brief:
    auto command="folder "+Settings::getInstance().current_project;
    manipulator.send_command(command);
  }

}
