
#include <gtkmm/application.h>
#include "graphics/window.hpp"

int main(int argc, char *argv[])
{
  auto app = Gtk::Application::create(argc, argv, "org.gtkmm.electrodynamics-simulator");

  maxwell::MainWindow window;

  return app->run(window);
}
