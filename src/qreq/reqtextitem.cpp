#include "qreq/reqtextitem.hpp"

namespace qreq{

///Wir wollen mehrere Strings halten:
///caption und maintext
///Der dem Item zugeordnete QVariant enthält also eine Stringlist in der Reihenfolge
///caption,maintext


ReqTextItem::ReqTextItem(){
  _maintext="";
  _caption="";
}

ReqTextItem::~ReqTextItem(){
}

void ReqTextItem::set_maintext(std::string const& maintext){
  _maintext=maintext;
}

void ReqTextItem::set_caption(std::string const& caption){
  _caption=caption;
}

std::string const& ReqTextItem::get_maintext()const{
  return _maintext;
}

std::string const& ReqTextItem::get_caption()const{
  return _caption;
}

void ReqTextItem::setData(QVariant const& value,int role){

}

}
