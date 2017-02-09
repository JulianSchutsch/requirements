#include "qreq/reqtextdelegate.hpp"

#include <sstream>

#include <QAbstractTextDocumentLayout>

#include "qreq/model.hpp"

#include <QKeyEvent>
#include <QPainter>

#include <iostream>

namespace qreq {

  ReqTextDelegate::ReqTextDelegate(QObject *parent) {}

  static const int innerBorder = 5;
  static const int gap = 5;

  struct ReqTextDelegate::CellGeometry {
    int textHeight;
    QRect frameRect;
    QRect headerRect;
    QRect bodyRect;
    QRect bodyTextRect;
    QSize fullSize;
    QFontMetrics fm;
    bool selected;
    int headerTextPos;
    CellGeometry(const QStyleOptionViewItem& option)
      : fm(option.font) {
      selected = option.state & QStyle::State_Selected;
      textHeight = fm.height();


      auto &rect = option.rect;
      fullSize = QSize(rect.width(), rect.height());
      auto width = rect.width();
      auto height = rect.height();

      auto innerLeft = rect.left() + innerBorder;
      auto innerRight = rect.right() - innerBorder;
      auto innerTop = rect.top() + innerBorder;
      auto innerHeight = height - 2 * innerBorder;
      auto boxTop = innerTop + textHeight + 2 * gap;
      auto innerBottom = rect.bottom() - innerBorder;
      auto innerWidth = width - 2 * innerBorder;
      auto boxHeight = height - textHeight - 4 * gap;

      frameRect = QRect(innerLeft, innerTop, innerWidth, innerHeight);
      headerRect = QRect(innerLeft, innerTop, innerWidth, textHeight + 2*gap);
      bodyRect = QRect(innerLeft, boxTop, innerWidth, boxHeight);
      bodyTextRect = QRect(innerLeft + gap, boxTop + gap, innerWidth - 2*gap, boxHeight - 2*gap);

      headerTextPos = innerLeft + gap;
    }

    CellGeometry(const QStyleOptionViewItem& option, const QModelIndex& index)
      : fm(option.font) {
      selected = option.state & QStyle::State_Selected;
      textHeight = fm.height();

      auto &rect = option.rect;

      auto &model = Model::getModel(index);
      auto node = model.getNodeFromModelIndex(index);
      QSizeF contentSize(0,0);
      if (node) {
        if(model.isEditing(index)) {
          QTextDocument document(model.editorContent());
          contentSize = document.size();
        } else {
          QTextDocument document(node->getContent().c_str());
          contentSize = document.size();
        }
      }

      auto boxWidth = contentSize.width() + 2*gap;
      auto innerWidth = boxWidth + 2*gap;
      auto width = innerWidth + 2*innerBorder;

      auto boxHeight = contentSize.height() + 2*gap;
      auto innerHeight = boxHeight + textHeight + 2 * gap;
      auto height = innerHeight + 2 * innerBorder;

      auto innerLeft = rect.left() + innerBorder;
      auto innerTop = rect.top() + innerBorder;
      auto boxTop = innerTop + textHeight + 2 * gap;

      frameRect = QRect(innerLeft, innerTop, innerWidth, innerHeight);
      headerRect = QRect(innerLeft, innerTop, innerWidth, textHeight + 2*gap);
      bodyRect = QRect(innerLeft, boxTop, innerWidth, boxHeight);
      bodyTextRect = QRect(innerLeft + gap, boxTop + gap, innerWidth - 2*gap, boxHeight - 2*gap);
      fullSize = QSize(width, height);

      headerTextPos = innerLeft + gap;
    }

    void headerTextOut(QPainter *painter, const std::string& str, const QColor& color) {
      auto textWidth = fm.width(str.c_str());
      painter->setPen(color);
      painter->drawText(QRect(headerTextPos, headerRect.top()+gap, textWidth, textHeight), Qt::AlignTop, str.c_str());
      headerTextPos += textWidth + gap;
    }
  };

  void ReqTextDelegate::paintFrame(QPainter* painter, CellGeometry& geometry) const {
    if(geometry.selected) {
      painter->fillRect(geometry.frameRect, QColor(255, 255, 0));
    }
    painter->drawRect(geometry.headerRect);
    painter->drawRect(geometry.bodyRect);
  }

  void ReqTextDelegate::paintRequirementAcceptance(QPainter* painter, CellGeometry& geometry, const ::requirements::batch::Response& model, ::requirements::Id nodeId) const {
    if(model.requirements->has(nodeId)) {
      const auto& requirement = model.requirements->get(nodeId);
      if(requirement.isCoveredByAcceptance()) {
        if(requirement.isTreeCoveredByAcceptance()) {
          geometry.headerTextOut(painter, "[A]", QColor(0, 255, 0));
        } else {
          geometry.headerTextOut(painter, "[A]", QColor(0,0,0));
        }
      } else {
        geometry.headerTextOut(painter, "[A]", QColor(255, 0, 0));
      }
    }
  }

  void ReqTextDelegate::paintNodeDescription(QPainter* painter, CellGeometry& geometry, const ::requirements::batch::Response& model, ::requirements::Id nodeId) const {
    std::string caption;
    QColor captionColor(0,0,255);
    if(model.shortcuts->has(nodeId)) {
      captionColor = QColor(0,128,0);
      caption = model.shortcuts->get(nodeId);
    } else {
      caption = ::requirements::id_to_string(nodeId);
    }
    if(model.errors->has(nodeId)) {
      captionColor = QColor(255,0,0);
      caption+=" " + model.errors->get(nodeId);
    }
    geometry.headerTextOut(painter, caption, captionColor);
  }

  void ReqTextDelegate::paintAcceptanceAccepts(QPainter* painter, CellGeometry& geometry, const ::requirements::batch::Response& model, ::requirements::Id nodeId) const {
    if(!model.acceptances->has(nodeId)) {
      return;
    }
    std::stringstream ss;
    ss<<"->";
    auto& acceptance = model.acceptances->get(nodeId);
    for(auto& accepts: acceptance.getAccepts()) {
      if(model.shortcuts->has(accepts)) {
        ss<<" "<<model.shortcuts->get(accepts);
      }
    }
    geometry.headerTextOut(painter, ss.str().c_str(), QColor(0,0,0));
  }

  void ReqTextDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, QModelIndex const &index) const {

    auto &model = Model::getModel(index);
    auto node = model.getNodeFromModelIndex(index);
    if (!node) {
      return;
    }

    painter->save();

    CellGeometry geometry(option);
    paintFrame(painter, geometry);

    auto& imodel = model.getModel();

    paintRequirementAcceptance(painter, geometry, model.getModel(), node->getId());
    paintNodeDescription(painter, geometry, model.getModel(), node->getId());
    paintAcceptanceAccepts(painter, geometry, model.getModel(), node->getId());

    if(!model.isEditing(index)) {
      QTextDocument document(node->getContent().c_str());
      painter->translate(geometry.bodyTextRect.left(), geometry.bodyTextRect.top());
      document.drawContents(painter);
    }

    painter->restore();

  }

  QSize ReqTextDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
    CellGeometry geometry(option, index);
    return geometry.fullSize;
  }

  void ReqTextDelegate::editorDestroyed(QObject *obj) {
    if(currentModel) {
      currentModel->clearCurrentEditor();
    }
  }

  QWidget * ReqTextDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    auto *editor = new QTextEdit(parent);
    connect(editor, SIGNAL(destroyed(QObject*)), this, SLOT(editorDestroyed(QObject*)));
    editor->setFrameStyle(QFrame::NoFrame);
    editor->setWordWrapMode(QTextOption::WrapMode::NoWrap);
    editor->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    editor->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
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
    CellGeometry geometry(option);
    editor->setGeometry(geometry.bodyTextRect);
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

