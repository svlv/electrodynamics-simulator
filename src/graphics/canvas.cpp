#include "canvas.hpp"

#include "arrow.hpp"
#include "constants.hpp"
#include "point.hpp"
#include "utils.hpp"

#include <gtkmm.h>
#include <iostream>
namespace maxwell
{

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

    double cur_x = arrow_delta / 2.0;
    double cur_y = arrow_delta / 2.0;

    while (cur_x + arrow_delta / 2.0 < width) {
        while (cur_y + arrow_delta / 2.0 < height) {
            _arrows.emplace_back(default_arrow_size, point(cur_x, cur_y), 0.0);
            cur_y += arrow_delta;
        }
        cur_x += arrow_delta;
        cur_y = arrow_delta / 2.0;
    }
}

void Canvas::_draw_arrows(const size& sz,
                          const Cairo::RefPtr<Cairo::Context>& cr)
{
    if (_draw_lines_flag && !_charges.empty()) {
        cr->save();

        for (auto& arr : _arrows) {
            const auto& coord = arr.get_coord();
            arr.rotate(
                get_angle(_charges.getCos(coord), _charges.getSin(coord)));

            if (arr.is_selected()) {
                Gdk::Cairo::set_source_rgba(cr, highlight_arrow_color);
                draw_line(arr.get_coord(), true, sz, cr);
                draw_line(arr.get_coord(), false, sz, cr);
            } else {
                Gdk::Cairo::set_source_rgba(cr, arrow_color);
            }
            arr.draw(cr, fill_arrow);
        }
        cr->restore();
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
        double dx = _charges.getCos(pos) * line_delta;
        double dy = _charges.getSin(pos) * line_delta;
        if (fabs(dx) < 1.0 && fabs(dy) < 1.0) {
            break;
        }
        if (positive) {
            pos += point(dx, dy);
        } else {
            pos -= point(dx, dy);
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

    const auto& posCharges = _charges.getPositiveCharges();
    const auto& negCharges = _charges.getNegativeCharges();

    _draw_arrows(sz, cr);

    if (_draw_lines) {
        for (const auto& charge : posCharges) {
            for (size_t i = 0; i < lines_per_charge; ++i) {
                const auto& charge_coord = charge.get_coord();
                auto pos = point(
                    charge_coord.x +
                        cos(i * 2 * M_PI / lines_per_charge) * line_delta,
                    charge_coord.y +
                        sin(i * 2 * M_PI / lines_per_charge) * line_delta);
                draw_line(pos, true, sz, cr);
            }
        }

        for (const auto& charge : negCharges) {
            for (size_t i = 0; i < lines_per_charge; ++i) {
                const auto& charge_coord = charge.get_coord();
                auto pos = point(
                    charge_coord.x +
                        cos(i * 2 * M_PI / lines_per_charge) * line_delta,
                    charge_coord.y +
                        sin(i * 2 * M_PI / lines_per_charge) * line_delta);
                draw_line(pos, false, sz, cr);
            }
        }
    }
    cr->restore();

    // draw charges
    cr->save();

    auto drawArc = [&cr](const charge& charge) {
        const auto& charge_coord = charge.get_coord();
        cr->arc(charge_coord.x, charge_coord.y, 10.0f, 0.0f, 2 * M_PI);
        cr->fill();
    };

    Gdk::Cairo::set_source_rgba(cr, positive_charge_color);
    std::for_each(posCharges.cbegin(), posCharges.cend(), drawArc);

    Gdk::Cairo::set_source_rgba(cr, negative_charge_color);
    std::for_each(negCharges.cbegin(), negCharges.cend(), drawArc);

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

    // std::cout << "Mouse press\n";
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
