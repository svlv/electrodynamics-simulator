#include "canvas.hpp"

#include "arrow.hpp"
#include "constants.hpp"
#include "point.hpp"
#include "utils.hpp"

#include <gtkmm.h>
#include <iostream>
namespace maxwell
{

class context_guard
{
  public:
    explicit context_guard(const Cairo::RefPtr<Cairo::Context>& cr) : _cr(cr)
    {
        _cr->save();
    }
    ~context_guard() { _cr->restore(); }

  private:
    const Cairo::RefPtr<Cairo::Context>& _cr;
};

Canvas::Canvas()
{
    add_events(Gdk::BUTTON_PRESS_MASK);
    add_events(Gdk::POINTER_MOTION_MASK);
    add_events(Gdk::KEY_PRESS_MASK);
    property_can_focus() = true;
}

void Canvas::_init_arrows(int width, int height)
{
    _arrows.clear();

    auto pos = point(arrow_delta / 2.0, arrow_delta / 2.0);

    while (pos.x + arrow_delta / 2.0 < width) {
        while (pos.y + arrow_delta / 2.0 < height) {
            _arrows.emplace_back(default_arrow_size, pos, 0.0);
            pos.y += arrow_delta;
        }
        pos.x += arrow_delta;
        pos.y = arrow_delta / 2.0;
    }
}

void Canvas::_draw_arrows(const size& sz,
                          const Cairo::RefPtr<Cairo::Context>& cr)
{
    if (_draw_lines_flag && !_charges.empty()) {
        const auto guard = context_guard(cr);
        Gdk::Cairo::set_source_rgba(cr, arrow_color);

        for (auto& arr : _arrows) {
            const auto& coord = arr.get_coord();
            arr.rotate(_charges.get_angle(coord));

            if (arr.is_selected()) {
                const auto guard = context_guard(cr);
                Gdk::Cairo::set_source_rgba(cr, highlight_arrow_color);
                draw_line(coord, true, sz, cr);
                draw_line(coord, false, sz, cr);
                arr.draw(cr, fill_arrow);
                continue;
            }

            arr.draw(cr, fill_arrow);
        }
    }
}

void Canvas::draw_line(point pos, bool positive, const size& sz,
                       const Cairo::RefPtr<Cairo::Context>& cr)
{
    const auto valid_position = [&pos, &sz]() {
        return pos.x > 0 && pos.x < sz.width && pos.y > 0 && pos.y < sz.height;
    };

    cr->move_to(pos.x, pos.y);
    // iteration counter is used because sometimes it's impossible for line to
    // leave a room
    for (size_t i = 0; i < 1000 && valid_position(); ++i) {
        auto end = positive ? _charges.isComeToNegative(pos)
                            : _charges.isComeToPositive(pos);
        if (end) {
            cr->line_to(end->x, end->y);
            break;
        }
        const auto delta = point(_charges.getCos(pos) * line_delta,
                                 _charges.getSin(pos) * line_delta);
        if (fabs(delta.x) < 1.0 && fabs(delta.y) < 1.0) {
            break;
        }
        if (positive) {
            pos += delta;
        } else {
            pos -= delta;
        }
        cr->line_to(pos.x, pos.y);
    }
    cr->stroke();
}

bool Canvas::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
    Gtk::Allocation allocation = get_allocation();
    const auto sz = size(allocation.get_width(), allocation.get_height());

    // draw background
    cr->save();

    Gdk::Cairo::set_source_rgba(cr, bg_color);
    cr->paint();
    cr->restore();

    // draw lines
    cr->save();
    cr->set_line_width(line_width);

    const auto& pos_charges = _charges.getPositiveCharges();
    const auto& neg_charges = _charges.getNegativeCharges();

    _draw_arrows(sz, cr);

    const auto get_begin = [](const point& coord, size_t idx) -> point {
        return point(
            coord.x + cos(idx * 2 * M_PI / lines_per_charge) * line_delta,
            coord.y + sin(idx * 2 * M_PI / lines_per_charge) * line_delta);
    };

    if (_draw_lines) {
        for (const auto& charge : pos_charges) {
            for (size_t idx = 0; idx < lines_per_charge; ++idx) {
                draw_line(get_begin(charge.get_coord(), idx), true, sz, cr);
            }
        }

        for (const auto& charge : neg_charges) {
            for (size_t idx = 0; idx < lines_per_charge; ++idx) {
                draw_line(get_begin(charge.get_coord(), idx), false, sz, cr);
            }
        }
    }
    cr->restore();

    // draw charges
    cr->save();

    auto draw_arc = [&cr](const charge& charge) {
        const auto& charge_coord = charge.get_coord();
        cr->arc(charge_coord.x, charge_coord.y, 10.0f, 0.0f, 2 * M_PI);
        cr->fill();
    };

    Gdk::Cairo::set_source_rgba(cr, positive_charge_color);
    std::for_each(pos_charges.cbegin(), pos_charges.cend(), draw_arc);

    Gdk::Cairo::set_source_rgba(cr, negative_charge_color);
    std::for_each(neg_charges.cbegin(), neg_charges.cend(), draw_arc);

    cr->restore();

    return true;
}

bool Canvas::on_button_press_event(GdkEventButton* event)
{
    if (event->type == GDK_BUTTON_PRESS) {
        if (event->button == 1) {
            _charges.emplace_back(charge::type::positive,
                                  point(event->x, event->y), 1.0);
            queue_draw();
        } else if (event->button == 3) {
            _charges.emplace_back(charge::type::negative,
                                  point(event->x, event->y), -1.0);
            queue_draw();
        }
    }
    return false;
}

bool Canvas::on_key_press_event(GdkEventKey* event)
{
    if (event->keyval == GDK_KEY_l) {
        _draw_lines = !_draw_lines;
        queue_draw();
    } else if (event->keyval == GDK_KEY_c) {
        _charges.clear();
        queue_draw();
    } else if (event->keyval == GDK_KEY_a) {
        _draw_lines_flag = !_draw_lines_flag;
        queue_draw();
    }
    return false;
}

bool Canvas::on_motion_notify_event(GdkEventMotion* event)
{
    const auto coord = point(event->x, event->y);
    for (auto& arr : _arrows) {
        arr.select(arr.is_hint(coord));
    }
    queue_draw();
    return false;
}

void Canvas::on_size_allocate(Gtk::Allocation& allocation)
{
    _init_arrows(allocation.get_width(), allocation.get_height());

    Gtk::DrawingArea::on_size_allocate(allocation);
}

} // namespace maxwell
