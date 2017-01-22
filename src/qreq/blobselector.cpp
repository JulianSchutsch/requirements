#include "qreq/blobselector.hpp"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QStandardItem>
#include <QList>
#include <QModelIndex>

namespace qreq{

BlobSelector::BlobSelector(QWidget *parent) : QDialog(parent){
  setMinimumWidth(400);
  _blobmodel=new QStandardItemModel();
  _blobmodel->setColumnCount(1);
  _blobtree=new QTreeView(this);
  _blobtree->setModel(_blobmodel);
  _blobtree->setHeaderHidden(true);
  connect(_blobtree,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(on_item_doubleclicked(QModelIndex)));

  QPushButton *okbutton=new QPushButton(tr("OK"));
  connect(okbutton,SIGNAL(clicked()),this,SLOT(on_okbutton_clicked()));
  QPushButton *cancelbutton=new QPushButton(tr("OK"));
  connect(cancelbutton,SIGNAL(clicked()),this,SLOT(on_cancelbutton_clicked()));

  //Pack all things
  QHBoxLayout *bottombuttonbox=new QHBoxLayout();
  bottombuttonbox->addStretch();
  bottombuttonbox->addWidget(okbutton);
  bottombuttonbox->addWidget(cancelbutton);
  QVBoxLayout *vbox=new QVBoxLayout();
  vbox->addWidget(_blobtree);
  vbox->addLayout(bottombuttonbox);
  setLayout(vbox);
}

BlobSelector::~BlobSelector(){
}

std::string BlobSelector::get_selected_blob(){
  std::string retval="";
  QModelIndex index=_blobtree->currentIndex();
  QStandardItem* item=_blobmodel->itemFromIndex(index);
  retval=item->text().toStdString();

  return retval;
}

void BlobSelector::append(std::string const& blob){
  QStandardItem *parent_item=_blobmodel->invisibleRootItem();
  QStandardItem *item_text=new QStandardItem(QString(blob.c_str()));
  item_text->setEditable(false);
  QList<QStandardItem*> item;
  item << item_text;
  parent_item->appendRow(item);
  _blobtree->resizeColumnToContents(0);
}

void BlobSelector::on_okbutton_clicked(){
  accept();
}

void BlobSelector::on_cancelbutton_clicked(){
  reject();
}

void BlobSelector::on_item_doubleclicked(QModelIndex const& index){
  (void)index;

  accept();
}

}
