#include "qreq/reqtextdelegate.hpp"

#include <sstream>

#include <QAbstractTextDocumentLayout>

#include "qreq/model.hpp"

#include <QKeyEvent>
#include <QPainter>
#include <QScrollBar>

#include <iostream>

namespace qreq {

  ReqTextDelegate::ReqTextDelegate(QTreeView* a_treeView)
    : treeView(a_treeView) {
    connect(treeView->horizontalScrollBar(), SIGNAL(rangeChanged(int, int)), this, SLOT(horizontalSliderRangeChanged(int, int)));
    connect(treeView->verticalScrollBar(), SIGNAL(rangeChanged(int, int)), this, SLOT(verticalSliderRangeChanged(int, int)));
  }

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
      auto innerTop = rect.top() + innerBorder;
      auto innerHeight = height - 2 * innerBorder;
      auto boxTop = innerTop + textHeight + 2 * gap;
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
    (void)obj;
    if(currentModel) {
      currentModel->clearCurrentEditor();
    }
  }

  QWidget * ReqTextDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    (void)option;
    (void)index;
    auto *editor = new QTextEdit(parent);
    connect(editor, SIGNAL(destroyed(QObject*)), this, SLOT(editorDestroyed(QObject*)));
    connect(editor, SIGNAL(cursorPositionChanged()), this, SLOT(cursorPositionChanged()));
    connect(editor, SIGNAL(textChanged()), this, SLOT(textChanged()));
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
    (void)model;
    auto *widget = static_cast<QTextEdit *>(editor);
    auto& pmodel = Model::getModel(index);
    pmodel.updateContent(index, widget->toPlainText().toStdString());
  }

  void ReqTextDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                                             const QModelIndex &) const {
    CellGeometry geometry(option);
    editor->setGeometry(geometry.bodyTextRect);
  }

  void ReqTextDelegate::horizontalSliderRangeChanged(int min, int max) {
    (void)min;
    (void)max;
    makeCursorVisible();
  }

  void ReqTextDelegate::verticalSliderRangeChanged(int min, int max) {
    (void)min;
    (void)max;
    makeCursorVisible();
  }

  void ReqTextDelegate::makeRegionVisible(QRect r, int margin) {
    auto hscroll = treeView->horizontalScrollBar();
    auto vscroll = treeView->verticalScrollBar();
    auto width = treeView->width()-(vscroll->isVisible()?vscroll->width():0);
    auto height = treeView->height()-(hscroll->isVisible()?hscroll->height():0);
    auto hvalue = hscroll->value();
    auto vvalue = vscroll->value();
    auto newhvalue = hvalue;
    auto newvvalue = vvalue;
    if(hvalue+margin>=r.left()) {
      newhvalue = std::max(r.left()-margin, 0);
    }
    if(hvalue+width-margin<r.right()) {
      newhvalue = std::min(r.right()+margin-width-1, hscroll->maximum());
    }
    if(newhvalue!=hvalue) {
      hscroll->setSliderPosition(newhvalue);
    }

    if(vvalue+margin>=r.top()) {
      newvvalue = std::max(r.top()-margin, 0);
    }
    if(vvalue+height-margin<r.bottom()) {
      newvvalue = std::min(r.bottom()+margin-height-1, vscroll->maximum());
    }
    if(newvvalue!=vvalue) {
      vscroll->setSliderPosition(newvvalue);
    }
  }

  void ReqTextDelegate::makeCursorVisible() {
    if(currentModel==nullptr) {
      return;
    }
    auto currentEditor = currentModel->getCurrentEditor();
    if(currentEditor==nullptr) {
      return;
    }
    QTextEdit* editor = static_cast<QTextEdit*>(currentEditor);
    auto innerRect = editor->cursorRect();
    auto hscroll = treeView->horizontalScrollBar();
    auto vscroll = treeView->verticalScrollBar();
    auto hvalue = hscroll->value();
    auto vvalue = vscroll->value();
    auto x = hvalue+editor->pos().x();
    auto y = vvalue+editor->pos().y();
    makeRegionVisible(QRect(innerRect.left()+x, innerRect.top()+y, innerRect.width(), innerRect.height()), 20);
  }

  void ReqTextDelegate::cursorPositionChanged() {
    makeCursorVisible();
  }

  void ReqTextDelegate::textChanged() {
    if(currentModel!=nullptr) {
      emit sizeHintChanged(currentModel->editedIndex());
    }
  }

  bool ReqTextDelegate::eventFilter(QObject *peditor, QEvent *event) {
    QTextEdit* editor = static_cast<QTextEdit*>(peditor);
    if (event->type() == QEvent::KeyPress) {
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

