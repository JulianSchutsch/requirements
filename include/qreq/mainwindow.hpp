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

#include "qreq/reqtree.hpp"
#include "qreq/commandline.hpp"
#include "qreq/threadconnector.hpp"

#include "requirements/batch/response.hpp"

#include "requirements/node.hpp"
#include "requirements/nodecollection.hpp"

namespace qreq{

const int COLUMN_COUNT=2;
const int COLUMN_TEXT=0;
const int COLUMN_ID=1;
///
/// Das Hauptfenster.
///
class MainWindow : public QMainWindow{
Q_OBJECT
  ReqTree *_reqtree;
  QStandardItemModel *_reqmodel;
  CommandLine *_commandline;
  QTimer* timer;

  ThreadConnector _threadconnector;
  void generate_elements();
  void generate_view();
  void generate_menu();
  void generate_menu_recent();

  void set_current_project(std::string const& filename);
  void load_current_project();

  //Backend functions

  ::requirements::batch::Response modelState;
  void printtree(std::string const& uuid_to_jump="");
  void add_children_to_tree(QStandardItem *parent_item,const requirements::NodePtr& node);
  void add_child_to_tree(QStandardItem *parent_item,const requirements::NodePtr& node);
  void new_node(bool sibling=true,bool copy_content=false);
  void commit_to_collection(std::string const& uuid, std::string const& content);
  std::string newblob(std::string sourcefilename);

  //Tree functions
  void set_focus_to_uuid(QStandardItem *parent_item, std::string const& uuid);
  std::string get_uuid_by_modelindex(const QModelIndex& index);
  std::string get_text_by_modelindex(const QModelIndex& index);
  QStandardItem* get_parent_item_by_modelindex(const QModelIndex& index);
  void add_blob_to_row(QModelIndex const& index,std::string const& blobtext);

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
  void on_openact_triggered(QString const& filename);
  void on_reqtree_expanded(const QModelIndex& i);
  void on_reqmodel_item_changed(QStandardItem* item);
  void on_reqtree_ctrl_left(const QModelIndex& i);
  void on_reqtree_ctrl_right(const QModelIndex& i);
  void on_reqtree_ctrl_up(const QModelIndex& i);
  void on_reqtree_ctrl_down(const QModelIndex& i);
  void on_reqtree_alt_return(const QModelIndex& i);
  void on_commandline_return(std::string const& command);

protected:
  void keyPressEvent(QKeyEvent *event);
  //bool eventFilter(QObject *object, QEvent *event);
public:
  MainWindow();
  ~MainWindow();
};

}
