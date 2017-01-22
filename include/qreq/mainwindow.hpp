#pragma once

#include <QMainWindow>
#include <QKeyEvent>
#include <QTreeView>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QString>
#include <QModelIndex>
#include <QLineEdit>

#include "qreq/reqtree.hpp"
#include "qreq/commandline.hpp"
#include "qreq/threadconnector.hpp"

#include "requirements/node.hpp"
#include "requirements/nodecollection.hpp"
#include "requirements/istorage.hpp"
#include "requirements/storage/text.hpp"

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

  ThreadConnector _threadconnector;
  void generate_elements();
  void generate_view();
  void generate_menu();
  void generate_menu_recent();

  void set_current_project(std::string const& filename);
  void load_current_project();

  //Backend functions
  std::unique_ptr<::requirements::storage::Text> _currentStorage;
  void init_project();
  //void init_collection();
  void store_collection();
  void printtree(std::string const& uuid_to_jump="");
  void add_children_to_tree(QStandardItem *parent_item,const requirements::NodePtr& node);
  void add_child_to_tree(QStandardItem *parent_item,const requirements::NodePtr& node);
  void new_node(bool copy_content=false);
  requirements::NodePtr get_node_for_uuid(std::string const& uuid);
  void commit_to_collection(std::string const& uuid, std::string const& content);
  std::string newblob(std::string sourcefilename);
  void add_new_brother_for(std::string const& uuid);

  //Tree functions
  void set_focus_to_uuid(QStandardItem *parent_item, std::string const& uuid);
  std::string get_uuid_by_modelindex(const QModelIndex& index);
  std::string get_text_by_modelindex(const QModelIndex& index);
  QStandardItem* get_parent_item_by_modelindex(const QModelIndex& index);
  void add_blob_to_row(QModelIndex const& index,std::string const& blobtext);

private slots:
  void on_f1button_clicked();
  void on_f2button_clicked();
  void on_f3button_clicked();
  void on_f4button_clicked();
  void on_f5button_clicked();
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
