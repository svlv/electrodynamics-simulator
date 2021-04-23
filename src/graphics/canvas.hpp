#pragma once

#include "graphics/arrow.hpp"
#include "graphics/base_line.hpp"
#include "graphics/circle.hpp"
#include "physics/charges.hpp"
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
    bool on_button_release_event(GdkEventButton* event) override;
    bool on_key_press_event(GdkEventKey* event) override;
    bool on_motion_notify_event(GdkEventMotion* event) override;
    void on_size_allocate(Gtk::Allocation& allocation) override;

  private:
    void _init_arrows(int width, int height);
    void _init_lines();
    base_line_uptr _make_line(point pos, bool positive, const size& sz);
    void _draw_arrows(const size& sz, const Cairo::RefPtr<Cairo::Context>& cr);
    void _draw_charges(const Cairo::RefPtr<Cairo::Context>& ctx);
    void _draw_lines(const Cairo::RefPtr<Cairo::Context>& cr);
    void _draw_background(const Cairo::RefPtr<Cairo::Context>& cr);
    void _draw_grid(const Cairo::RefPtr<Cairo::Context>& cr);

    charges _charges;
    field _field;
    bool _draw_lines_flag = false;
    bool _draw_arrows_flag = true;
    std::vector<arrow> _arrows;
    std::vector<circle> _circles;
    std::vector<base_line_uptr> _lines;
    circle* _selected_circle;

    base_line::type _line_type = base_line::type::curve;
    double _line_delta = 10.0;
};

} // namespace maxwell
