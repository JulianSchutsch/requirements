#include "qreq/reqtextdelegate.hpp"
#include "qreq/model.hpp"

#include <QKeyEvent>
#include <QPainter>

#include <iostream>
#include <include/requirements/batch/response.hpp>

//TODO: Signal-Handling verbessern: F4 aus dem Hauptfenster muss direkt im TextEdit landen.
//Dort drin muss ctrl+Return abgefangen werden. Jetzt wird das noch auf der Ebene des ReqTextItemWidget gemacht

namespace qreq {

  ReqTextDelegate::ReqTextDelegate(QObject *parent) {}

  static const int innerBorder = 5;
  static const int gap = 5;

  void ReqTextDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, QModelIndex const &index) const {

    auto &model = Model::getModel(index);
    auto node = model.getNodeFromModelIndex(index);
    if (!node) {
      return;
    }

    auto &rect = option.rect;
    auto left = rect.x();
    auto top = rect.y();
    auto width = rect.width();
    auto height = rect.height();
    auto right = rect.right();
    auto bottom = rect.bottom();

    QFontMetrics fm(option.font);
    int textHeight = fm.height();

    auto innerLeft = left + innerBorder;
    auto innerRight = right - innerBorder;
    auto innerTop = top + innerBorder;
    auto innerHeight = height - 2 * innerBorder;
    auto boxTop = innerTop + textHeight + 2 * gap;
    auto innerBottom = bottom - innerBorder;
    auto innerWidth = width - 2 * innerBorder;
    auto boxHeight = height - textHeight - 4 * gap;

    painter->save();
    if(option.state & QStyle::State_Selected) {
      painter->fillRect(QRect(innerLeft, innerTop, innerWidth, innerHeight), QColor(255, 255,0));
    }
    painter->drawLine(QPoint(innerLeft, innerTop), QPoint(innerRight, innerTop));
    painter->drawLine(QPoint(innerLeft, innerTop), QPoint(innerLeft, boxTop));
    painter->drawLine(QPoint(innerRight, innerTop), QPoint(innerRight, boxTop));
    painter->drawLine(QPoint(innerLeft, boxTop), QPoint(innerRight, boxTop));
    painter->drawLine(QPoint(innerLeft, boxTop), QPoint(innerLeft, innerBottom));
    painter->drawLine(QPoint(innerRight, boxTop), QPoint(innerRight, innerBottom));
    painter->drawLine(QPoint(innerLeft, innerBottom), QPoint(innerRight, innerBottom));

    auto& imodel = model.getModel();
    auto id = node->getId();
    std::string caption;
    QColor captionColor(0,0,255);
    if(imodel.shortcuts->has(id)) {
      captionColor = QColor(0,128,0);
      caption = imodel.shortcuts->get(id);
    } else {
      caption = ::requirements::id_to_string(id);
    }
    if(imodel.errors->has(id)) {
      captionColor = QColor(255,0,0);
      caption+=" "+imodel.errors->get(id);
    }

    auto captionRect = QRect(innerLeft + gap, innerTop + gap, innerWidth - 2 * gap, textHeight);

    painter->setPen(captionColor);
    painter->drawText(captionRect, Qt::AlignTop, caption.c_str());

    auto boxRect = QRect(innerLeft + gap, boxTop + gap, innerWidth - 2 * gap, boxHeight);

    if(!model.isEditing(index)) {
      QTextDocument document(node->getContent().c_str());
      painter->translate(boxRect.left(), boxRect.top());
      document.drawContents(painter);
    }

    painter->restore();

  }

  QSize ReqTextDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
    auto &model = Model::getModel(index);
    auto node = model.getNodeFromModelIndex(index);
    if (!node) {
      return QSize(200, 200);
    }
    QSizeF size;
    if(model.isEditing(index)) {
      size = model.editorSize();
    } else {
      QTextDocument document(node->getContent().c_str());
      document.setTextWidth(option.rect.width());
      size = document.size();
    }
    return QSize(option.rect.width(), size.height() + 50);
  }

  void ReqTextDelegate::editorDestroyed(QObject *obj) {
    if(currentModel) {
      currentModel->clearCurrentEditor();
    }
  }

  QWidget *
  ReqTextDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    auto *editor = new QTextEdit(parent);
    connect(editor, SIGNAL(destroyed(QObject*)), this, SLOT(editorDestroyed(QObject*)));
    editor->setFrameStyle(QFrame::NoFrame);
    return editor;
  }

  void ReqTextDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
    auto *item_widget = static_cast<QTextEdit *>(editor);
    auto &model = Model::getModel(index);
    auto node = model.getNodeFromModelIndex(index);
    if (!node) {
      std::cout << "Cannot initialize editor, no data" << std::endl;
      return;
    }
    item_widget->setText(node->getContent().c_str());
    currentModel = &model;
    model.setCurrentEditor(index, static_cast<QTextEdit*>(editor));
  }

  void ReqTextDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
    auto *widget = static_cast<QTextEdit *>(editor);
    auto& pmodel = Model::getModel(index);
    pmodel.updateContent(index, widget->toPlainText().toStdString());
  }

  void ReqTextDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                                             const QModelIndex &) const {
    auto &rect = option.rect;
    auto left = rect.x();
    auto top = rect.y();
    auto width = rect.width();
    auto height = rect.height();

    QFontMetrics fm(option.font);
    int textHeight = fm.height();

    auto innerLeft = left + innerBorder;
    auto innerTop = top + innerBorder;
    auto boxTop = innerTop + textHeight + 2 * gap;
    auto innerWidth = width - 2 * innerBorder;
    auto boxHeight = height - textHeight - 4 * gap;

    editor->setGeometry(QRect(innerLeft+gap, boxTop+gap, innerWidth-2*gap, boxHeight-2*gap));
  }

  bool ReqTextDelegate::eventFilter(QObject *editor, QEvent *event) {
    if (event->type() == QEvent::KeyPress) {
      if(currentModel!=nullptr) {
        emit sizeHintChanged(currentModel->editedIndex());
      }
      QKeyEvent *key_event = static_cast<QKeyEvent *>(event);
      if (key_event->modifiers() == Qt::ControlModifier) {
        if (key_event->key() == Qt::Key_Return) {
          emit commitData(static_cast<QWidget *>(editor));
          emit closeEditor(static_cast<QWidget *>(editor), QAbstractItemDelegate::NoHint);
          return true;
        }
      }
    }
    return false;
  }

}

