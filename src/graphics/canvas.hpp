#pragma once

#include "graphics/arrow.hpp"
#include "physics/field.hpp"
#include "utils.hpp"
#include <gtkmm/drawingarea.h>
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
    void _init_arrows(int width, int height);
    void _draw_arrows(const Cairo::RefPtr<Cairo::Context>& cr);
    // TODO is temporal method
    void draw_line(point pos, bool positive, const size& sz,
                   const Cairo::RefPtr<Cairo::Context>& cr);

    field _charges;
    bool _draw_lines = false;
    bool _draw_lines_flag = true;
    std::vector<arrow> _arrows;
};

} // namespace maxwell
