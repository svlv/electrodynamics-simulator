#pragma once

#include <gtkmm/entry.h>

namespace elfield
{
class canvas;
class command_line : public Gtk::Entry
{
  public:
    explicit command_line(canvas& cnvs);

  private:
    bool on_key_press_event(GdkEventKey* event) override;

  private:
    canvas& _canvas;
};
} // namespace elfield
