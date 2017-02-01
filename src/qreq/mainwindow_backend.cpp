#include "qreq/mainwindow.hpp"
#include "qreq/settings.hpp"
#include "qreq/model.hpp"

#include "requirements/select.hpp"
#include "requirements/blob.hpp"
#include "requirements/commands/new.hpp"

#include <iostream>

namespace qreq{

void MainWindow::new_node(bool sibling,bool copy_content){
  ::requirements::Id id = ::requirements::generateRandomId();
  _threadconnector._batchthread.enqueue(std::make_unique<::requirements::commands::New>(id));
  // TODO: The new node should be inserted at a specific location, this requires a working model though.
  //       Issue a second command.
}

std::string MainWindow::newblob(std::string sourcefilename){
  // TODO: Send Command instead
  //std::string id = requirements::importBlob(*_currentStorage, sourcefilename);
  return "";
}

}
