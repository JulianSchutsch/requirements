#pragma once

#include <QMainWindow>
#include <QKeyEvent>
#include <QTreeView>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QString>
#include <QModelIndex>
#include <QLineEdit>
#include <QTimer>
#include <QListWidget>
#include <QStringListModel>

#include "qreq/reqtree.hpp"
#include "qreq/commandline.hpp"

#include "qreq/model.hpp"
#include "qreq/modelmanipulator.hpp"

namespace qreq {

  const int COLUMN_COUNT = 2;
  const int COLUMN_TEXT = 0;
  const int COLUMN_ID = 1;

  class MainWindow : public QMainWindow {
  Q_OBJECT
    ReqTree *_reqtree;
    CommandLine *_commandline;
    QTimer *timer;
    QListWidget *_messageList;

    void generate_elements();
    void generate_view();
    void generate_menu();
    void generate_menu_recent();
    void set_current_project(std::string const &filename);
    void load_current_project();

    Model model;
    ModelManipulator manipulator;

    void printMessage(const BatchMessage& message);

  private slots:

    void updateTimer();
    void on_f1button_clicked();
    void on_f2button_clicked();
    void on_f3button_clicked();
    void on_f4button_clicked();
    void on_f5button_clicked();
    void on_f6button_clicked();
    void on_f7button_clicked();
    void on_f8button_clicked();
    void on_f9button_clicked();
    void on_f10button_clicked();
    void on_newblobbutton_clicked();
    void on_linkblobbutton_clicked();
    void on_openact_triggered(QString const &filename);
    void on_reqtree_expanded(const QModelIndex &i);
    void on_reqtree_ctrl_left(const QModelIndex &i);
    void on_reqtree_ctrl_right(const QModelIndex &i);
    void on_reqtree_ctrl_up(const QModelIndex &i);
    void on_reqtree_ctrl_down(const QModelIndex &i);
    void on_reqtree_alt_return(const QModelIndex &i);
    void on_commandline_return(std::string const &command);

  protected:
    void keyPressEvent(QKeyEvent *event);

  public:
    MainWindow();
    ~MainWindow();
  };

}
