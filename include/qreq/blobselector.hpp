#pragma once

#include <QDialog>
#include <QTreeView>
#include <QStandardItemModel>
#include <QModelIndex>
#include <string>

namespace qreq{

class BlobSelector : public QDialog{
  Q_OBJECT
  QTreeView* _blobtree;
  QStandardItemModel *_blobmodel;
private slots:
  void on_okbutton_clicked();
  void on_cancelbutton_clicked();
  void on_item_doubleclicked(QModelIndex const& index);
public:
  BlobSelector(QWidget *parent);
  ~BlobSelector();
  std::string get_selected_blob();
  void append(std::string const& blob);
};

}
