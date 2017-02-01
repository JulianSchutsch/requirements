#pragma once

#include <QString>
#include <QStandardItem>

#include "requirements/id.hpp"
#include "requirements/batch/response.hpp"

#include <iostream>

namespace qreq {

  class ModelItem {
  private:
    static const int ROLE_MODEL=1000;
    static const int ROLE_ID=1001;
    ::requirements::Id id;
    ::requirements::batch::Response* model = nullptr;
  public:
    ::requirements::Id getId() const { return id; }
    bool valid() const {
      if(model==nullptr) {
        return false;
      }
      if(model->nodeCollection==nullptr) {
        std::cout<<"No Node Collection"<<std::endl;
        return false;
      }
      auto& collection = *model->nodeCollection.get();
      std::cout<<"Id exists?"<<collection.getNodeById(id)<<":"<<::requirements::id_to_string(id)<<std::endl;
      return collection.getNodeById(id)!=nullptr;
    }

    QString getContentAsQString() const { return model->nodeCollection->getNodeById(id)->getContent().c_str(); }

    void setContentFromQString(QString str) {
      model->nodeCollection->getNodeById(id)->updateContent(str.toStdString());
    }

    bool hasError() const { return model->errors->has(id); }
    QString getErrorAsQString() const { return model->errors->get(id).c_str(); }

    ModelItem(QModelIndex const& index) {
      model = static_cast<::requirements::batch::Response*>(index.data(Qt::UserRole + ROLE_MODEL).value<void*>());
      assert(model!=nullptr);
      std::string uuidStr = index.data(Qt::UserRole + ROLE_ID).toString().toUtf8().constData();
      bool uuidValid = ::requirements::string_to_id(uuidStr, id);
      assert(uuidValid==true);
    }

    ModelItem(::requirements::batch::Response& a_model, ::requirements::Id a_id)
      : id(a_id)
      , model(&a_model) {}

    void saveToQStandardItem(QStandardItem& item) {
      assert(model!=nullptr);
      item.setData(qVariantFromValue(static_cast<void*>(model)), Qt::UserRole + ROLE_MODEL);
      item.setData(QVariant(QString(requirements::id_to_string(id).c_str())), Qt::UserRole + ROLE_ID);
    }

    void saveToModel(QAbstractItemModel *amodel, const QModelIndex &index) {
      amodel->setData(index, qVariantFromValue(static_cast<void*>(model)), Qt::UserRole + ROLE_MODEL);
      amodel->setData(index, QVariant(QString(requirements::id_to_string(id).c_str())), Qt::UserRole + ROLE_ID);
    }

  };

}