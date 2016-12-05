#include <gtkmm/application.h>
#include "greq/mainwindow.hpp"

int main(int argc, char *argv[])
{
  //ApplicationID is "greq.application" to prevent a message like
  //GLib-GIO-CRITICAL **: g_application_set_application_id: assertion 'application_id == NULL || g_application_id_is_valid (application_id)' failed
  auto app = Gtk::Application::create(argc, argv,"greq.application");

  MainWindow mainwindow;
  
  return app->run(mainwindow);
}
