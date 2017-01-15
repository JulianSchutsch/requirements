#pragma once

#include <QMainWindow>
#include <QKeyEvent>
#include <QTreeView>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QString>
#include <QModelIndex>

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
  QTreeView *_reqtree;
  QStandardItemModel *_reqmodel;

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

  //Tree functions
  void set_focus_to_uuid(QStandardItem *parent_item, std::string const& uuid);
  //void commit_to_collection(std::string const& uuid, std::string const& content);
  //void add_new_brother_for(std::string const& uuid);
  //void new_node(bool copy_content=false);
  //std::string newblob(std::string sourcefilename);

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
  //void on_openact_triggered(unsigned namepos);
  void on_openact_triggered(QString const& filename);
  void on_reqtree_expanded(const QModelIndex& i);
  void on_reqmodel_item_changed(QStandardItem* item);
protected:
  void keyPressEvent(QKeyEvent *event);
  //bool eventFilter(QObject *object, QEvent *event);
public:
  MainWindow();
  ~MainWindow();
};

}
