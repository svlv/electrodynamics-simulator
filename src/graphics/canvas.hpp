#pragma once

#include "physics/charge.hpp"
#include <gtkmm/drawingarea.h>

namespace maxwell
{

class Canvas : public Gtk::DrawingArea
{
  public:
    Canvas();
    virtual ~Canvas() = default;

  protected:
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;

    bool on_button_press_event(GdkEventButton* event) override;
    bool on_key_press_event(GdkEventKey* event) override;

  private:
    Charges _charges;
    bool _draw_lines = false;
};

} // namespace maxwell
