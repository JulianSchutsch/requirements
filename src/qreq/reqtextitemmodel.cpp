#include "qreq/reqtextitemmodel.hpp"
#include "qreq/reqtextitem.hpp"

namespace qreq{

ReqTextItemModel::ReqTextItemModel(QObject *parent) : QStandardItemModel(parent){
  _reqtextitemdata.maintext="";
  _reqtextitemdata.caption="";
}

ReqTextItemModel::~ReqTextItemModel(){
}

QVariant ReqTextItemModel::data(const QModelIndex& index, int role)const{
}

bool ReqTextItemModel::setData(QModelIndex const& index, QVariant const& value, int role){
  if (!index.isValid())
    return false;
  ReqTextItem *item = static_cast<ReqTextItem*>(itemFromIndex(index));
  if (item == 0)
    return false;
  item->setData(value, role);
    return true;
}

}
