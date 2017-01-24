#pragma once

#include <QStandardItem>
#include <QVariant>
#include <string>

namespace qreq{

class ReqTextItem : public QStandardItem{
  std::string _maintext;
  std::string _caption;
public:
  ReqTextItem();
  ~ReqTextItem();
  void set_maintext(std::string const& maintext);
  void set_caption(std::string const& caption);
  std::string const& get_maintext()const;
  std::string const& get_caption()const;
  void setData(QVariant const& value,int role=Qt::UserRole+1);
};

}
