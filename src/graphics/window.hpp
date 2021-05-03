#pragma once

#include "canvas.hpp"

#include <gtkmm/frame.h>
#include <gtkmm/window.h>

namespace elfield
{

class main_window : public Gtk::Window
{
  public:
    main_window();
    virtual ~main_window() = default;

  protected:
    canvas _canvas;
    Gtk::Frame _frame;

  private:
    bool on_key_press_event(GdkEventKey* event) override;
};

} // namespace elfield
