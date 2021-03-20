#pragma once

#include "physics/field.hpp"
#include <gtkmm/drawingarea.h>
#include "graphics/arrow.hpp"
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
    field _charges;
    bool _draw_lines = false;
    //arrow _arrow;
};

} // namespace maxwell
