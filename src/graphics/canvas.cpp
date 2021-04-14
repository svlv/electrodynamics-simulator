#include "canvas.hpp"

#include "arrow.hpp"
#include "constants.hpp"
#include "point.hpp"
#include "utils.hpp"
#include "context_guard.hpp"

#include <gtkmm.h>
#include <iostream>
namespace maxwell
{

Canvas::Canvas() : _field(_charges), _selected_circle(nullptr)
{
    add_events(Gdk::BUTTON_PRESS_MASK);
    add_events(Gdk::BUTTON_RELEASE_MASK);
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
    if (_draw_arrows_flag && !_charges.empty()) {
        const auto guard = context_guard(cr);
        Gdk::Cairo::set_source_rgba(cr, arrow_color);

        for (auto& arr : _arrows) {
            const auto& coord = arr.get_coord();
            arr.rotate(_field.get_angle(coord));

            if (arr.is_selected()) {
                const context_guard guard(cr);
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
        auto* end = positive ? _charges.get_hint(pos, charge::type::negative, 10.0)
                            : _charges.get_hint(pos, charge::type::positive, 10.0);
        if (end) {
            const auto& coord = end->get_coord();
            cr->line_to(coord.x, coord.y);
            break;
        }
        const auto delta = point(_field.get_cos(pos) * line_delta,
                                 _field.get_sin(pos) * line_delta);
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

void Canvas::_draw_charges(const Cairo::RefPtr<Cairo::Context>& ctx)
{
    for (const auto& circle : _circles) {
      circle.draw(ctx);
    }
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

    const auto& pos_charges = _charges.get_positive_charges();
    const auto& neg_charges = _charges.get_negative_charges();

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

    _draw_charges(cr);

    return true;
}

bool Canvas::on_button_press_event(GdkEventButton* event)
{
    if (event->type == GDK_BUTTON_PRESS) {
        const auto coord = point(event->x, event->y);
        if (event->button == 1) {
            auto it = std::find_if(_circles.begin(), _circles.end(), [&coord](const auto& circle){
                return circle.is_hint(coord);
            });
            if (it != _circles.end()) {
              _selected_circle = &(*it);
            } else {
            _charges.emplace_back(charge::type::positive,
                                  coord, 1.0);
            _circles.emplace_back(std::ref(_charges.get_positive_charges().back()));
            queue_draw();
            }
        } else if (event->button == 3) {
            _charges.emplace_back(charge::type::negative,
                                  coord, -1.0);
            _circles.emplace_back(std::ref(_charges.get_negative_charges().back()));
            queue_draw();
        }
    }
    return false;
}

bool Canvas::on_button_release_event(GdkEventButton* event)
{
  _selected_circle = nullptr;
  return false;
}

bool Canvas::on_key_press_event(GdkEventKey* event)
{
    if (event->keyval == GDK_KEY_l) {
        _draw_lines = !_draw_lines;
        queue_draw();
    } else if (event->keyval == GDK_KEY_c) {
        _charges.clear();
        _circles.clear();
        _selected_circle = nullptr;
        queue_draw();
    } else if (event->keyval == GDK_KEY_a) {
        _draw_arrows_flag = !_draw_arrows_flag;
        queue_draw();
    }
    return false;
}

bool Canvas::on_motion_notify_event(GdkEventMotion* event)
{
    const auto coord = point(event->x, event->y);
    if (_selected_circle) {
      _selected_circle->move(coord);
    } else {
        for (auto& arr : _arrows) {
            arr.select(arr.is_hint(coord));
        }
        for (auto& circle : _circles) {
            circle.select(circle.is_hint(coord));
        }
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
