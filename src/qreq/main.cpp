#include "qreq/mainwindow.hpp"
//#include "settingsmanager.hpp"
#include <QApplication>

#include <iostream>

int main(int argc, char** argv){

  QApplication program(argc, argv);

  //SettingsManager
  //SettingsManager::instance()->load();

  qreq::MainWindow mainwindow;
  mainwindow.show();
  int retval=program.exec();

  //SettingsManager::instance()->store();
  return retval;
}
