#pragma once

#include <QMainWindow>
#include <QKeyEvent>
#include <QTreeView>
#include <QStandardItemModel>

namespace qreq{

///
/// Das Hauptfenster.
///
class MainWindow : public QMainWindow{
Q_OBJECT
  QTreeView *_reqtree;
  QStandardItemModel *_reqmodel;

  void generate_elements();
  void generate_view();
  void generate_menu();
private slots:
  void on_f1button_clicked();
  void on_f2button_clicked();
  void on_f3button_clicked();
  void on_f4button_clicked();
  void on_f5button_clicked();
  void on_f7button_clicked();
  void on_f8button_clicked();
  void on_f9button_clicked();
  void on_f10button_clicked();
  void on_newblobbutton_clicked();
  void on_linkblobbutton_clicked();
protected:
  void keyPressEvent(QKeyEvent *event);
  //bool eventFilter(QObject *object, QEvent *event);
public:
  MainWindow();
  ~MainWindow();
};

}
