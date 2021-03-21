#pragma once

#include "physics/field.hpp"
#include <gtkmm/drawingarea.h>
#include "graphics/arrow.hpp"
#include <vector>

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
    bool on_motion_notify_event(GdkEventMotion* event) override;
    void on_size_allocate(Gtk::Allocation& allocation) override;

  private:
    void _init_arrows();

    field _charges;
    bool _draw_lines = false;
    std::vector<arrow> _arrows;
};

} // namespace maxwell
