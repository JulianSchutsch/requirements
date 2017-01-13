#pragma once

#include <QMainWindow>

///
/// Das Hauptfenster.
///
class MainWindow : public QMainWindow{
Q_OBJECT

  void generate_elements();
  void generate_view();
  void generate_menu();

public:
  MainWindow();
  ~MainWindow();
};
