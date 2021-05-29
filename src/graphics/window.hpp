#pragma once

#include "canvas.hpp"
#include "graphics/widgets/command_line.hpp"

#include <gtkmm/box.h>
#include <gtkmm/window.h>

namespace elfield
{

class main_window : public Gtk::Window
{
  public:
    main_window();
    virtual ~main_window() = default;

  protected:
    bool on_key_press_event(GdkEventKey* event) override;

  private:
    void on_notification_from_worker_thread();

    canvas _canvas;
    Gtk::Box _box;
    command_line _cli;
};

} // namespace elfield
