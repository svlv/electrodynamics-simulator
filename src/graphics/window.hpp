#pragma once

#include "canvas.hpp"

#include <gtkmm/frame.h>
#include <gtkmm/window.h>

namespace elfield
{

class MainWindow : public Gtk::Window
{
  public:
    MainWindow();
    virtual ~MainWindow() = default;

  protected:
    Canvas _canvas;
    Gtk::Frame _frame;

  private:
    bool on_key_press_event(GdkEventKey* event) override;
};

} // namespace elfield
