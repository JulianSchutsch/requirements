#include "qreq/mainwindow.hpp"
#include <QApplication>

int main(int argc, char** argv){

  QApplication program(argc, argv);

  qreq::MainWindow mainwindow;
  mainwindow.show();
  int retval=program.exec();

  return retval;
}
