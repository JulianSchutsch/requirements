#include "qreq/reqtextdelegate.hpp"
#include "qreq/reqtextitemwidget.hpp"
#include "qreq/modelroles.hpp"

#include <QKeyEvent>
#include <QPainter>

#include <iostream>

//TODO: Signal-Handling verbessern: F4 aus dem Hauptfenster muss direkt im TextEdit landen.
//Dort drin muss ctrl+Return abgefangen werden. Jetzt wird das noch auf der Ebene des ReqTextItemWidget gemacht

namespace qreq{

ReqTextDelegate::ReqTextDelegate(QObject *parent)
 : QStyledItemDelegate(parent){
}

void ReqTextDelegate::paint(QPainter *painter,const QStyleOptionViewItem &option, QModelIndex const& index) const{
  // here we have active painter provided by caller
  // by the way - we can't use painter->save() and painter->restore()
  // methods cause we have to call painter->end() method before painting
  // the QWidget, and painter->end() method deletes
  // the saved parameters of painter

  // we have to save paint device of the provided painter to restore the painter
  // after drawing QWidget
  QPaintDevice* original_pdev_ptr = painter->device();

  // example of simple drawing (selection) before widget
  // highlight background if selected
  if (option.state & QStyle::State_Selected) painter->fillRect(option.rect, option.palette.highlight());
  // creating local QWidget (that's why i think it should be fasted, cause we
  // don't touch the heap and don't deal with a QWidget except painting)
  ReqTextItemWidget *item_widget=new ReqTextItemWidget();
  QString maintext=index.data(Qt::UserRole + ROLE_TEXT).toString();
  QString caption=index.data(Qt::UserRole + ROLE_CAPTION).toString();
  item_widget->set_caption(caption.toStdString());
  item_widget->set_maintext(maintext.toStdString());

  // geometry
  item_widget->setGeometry(option.rect);

  // here we have to finish the painting of provided painter, cause
  //     1) QWidget::render(QPainter *,...) doesn't work with provided external painter
  //          and we have to use QWidget::render(QPaintDevice *,...)
  //          which creates its own painter
  //     2) two painters can't work with the same QPaintDevice at the same time
  painter->end();
  // rendering of QWidget itself
  //region liegt relativ zu option.rect
  QPoint modifier=ReqTextItemWidget::region_for_delegate_modifier();
  int region_x=modifier.x();
  int region_y=modifier.y();
  int region_w=option.rect.width()-region_x;
  int region_h=option.rect.height()-region_y;
  //std::cout << "(" << region_x << "," << region_y << "," << region_w << "," << region_h << std::endl;
  item_widget->render(painter->device(), QPoint(option.rect.x(), option.rect.y()), QRegion(region_x, region_y, region_w, region_h), QWidget::RenderFlag::DrawChildren);


  // starting (in fact just continuing) painting with external painter, provided
  // by caller
  painter->begin(original_pdev_ptr);

}

QSize ReqTextDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const{
  QSize retval=QStyledItemDelegate::sizeHint(option,index);
  QSize modifier=ReqTextItemWidget::size_hint_for_delegate_modifier();
  retval.setHeight(retval.height()+modifier.height());
  retval.setWidth(retval.width()+modifier.width());
  return retval;
}

QWidget *ReqTextDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &) const{
  ReqTextItemWidget *editor=new ReqTextItemWidget(parent);

  return editor;
}

void ReqTextDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const{
  ReqTextItemWidget *item_widget=static_cast<ReqTextItemWidget*>(editor);
  QString maintext=index.data(Qt::UserRole + ROLE_TEXT).toString();
  QString caption=index.data(Qt::UserRole + ROLE_CAPTION).toString();
  item_widget->set_caption(caption.toStdString());
  item_widget->set_maintext(maintext.toStdString());

}

void ReqTextDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const{
  ReqTextItemWidget *widget=static_cast<ReqTextItemWidget*>(editor);
  QString value=widget->get_maintext().c_str();

  //Das führt zu mehrfachen itemChanged()-Signalen. Kann man das vielleicht in einen Aufruf stecken?
  model->setData(index, value, Qt::EditRole);  //TODO das kann entfallen, wenn wir endgültig auf dieses Delegate wechseln
  model->setData(index, value, Qt::UserRole+ROLE_TEXT);
}

void ReqTextDelegate::updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem &option, const QModelIndex &) const{
  editor->setGeometry(option.rect);
}

bool ReqTextDelegate::eventFilter(QObject *editor, QEvent *event){
  if (event->type() == QEvent::KeyPress) {
    QKeyEvent* key_event = static_cast<QKeyEvent*>(event);
    if(key_event->modifiers()==Qt::ControlModifier){
      if(key_event->key()==Qt::Key_Return){
        emit commitData(static_cast<QWidget*>(editor));
        emit closeEditor(static_cast<QWidget*>(editor),QAbstractItemDelegate::EditNextItem);
        return true;
      }
    }
  }

  return QStyledItemDelegate::eventFilter(editor,event);
}

}

