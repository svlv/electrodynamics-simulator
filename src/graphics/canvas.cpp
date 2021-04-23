#include "canvas.hpp"

#include "arrow.hpp"
#include "constants.hpp"
#include "context_guard.hpp"
#include "graphics/curve.hpp"
#include "graphics/line.hpp"
#include "graphics/square.hpp"
#include "point.hpp"
#include "utils.hpp"

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

base_line_uptr Canvas::_make_line(point pos, bool positive, const size& sz)
{
    const auto valid_position = [&pos, &sz]() {
        return pos.x > 0 && pos.x < sz.width && pos.y > 0 && pos.y < sz.height;
    };
    auto crv = [&]() -> base_line_uptr {
        switch (_line_type) {
        case base_line::type::line:
            return std::make_unique<line>(pos);
        case base_line::type::curve:
            return std::make_unique<curve>(pos);
        }
        return nullptr;
    }();
    for (size_t i = 0; i < 1000 && valid_position(); ++i) {
        auto end = positive
                       ? _charges.get_hint(pos, charge::type::negative, 10.0)
                       : _charges.get_hint(pos, charge::type::positive, 10.0);
        if (end) {
            const auto& coord = end->get_coord();
            crv->add_point(coord);
            break;
        }
        const auto delta = point(_field.get_cos(pos) * _line_delta,
                                 _field.get_sin(pos) * _line_delta);
        if (fabs(delta.x) < 1.0 && fabs(delta.y) < 1.0) {
            break;
        }
        if (positive) {
            pos += delta;
        } else {
            pos -= delta;
        }
        crv->add_point(pos);
    }
    crv->fill();
    return crv;
}

void Canvas::_init_lines()
{
    Gtk::Allocation allocation = get_allocation();
    const auto sz = size(allocation.get_width(), allocation.get_height());
    _lines.clear();
    const auto get_begin = [](const point& coord, size_t idx) -> point {
        return point(
            coord.x + cos(idx * 2 * M_PI / lines_per_charge) * line_delta,
            coord.y + sin(idx * 2 * M_PI / lines_per_charge) * line_delta);
    };
    for (const auto& charge : _charges.get_positive_charges()) {
        for (size_t idx = 0; idx < lines_per_charge; ++idx) {
            _lines.emplace_back(
                _make_line(get_begin(charge->get_coord(), idx), true, sz));
        }
    }
    for (const auto& charge : _charges.get_negative_charges()) {
        for (size_t idx = 0; idx < lines_per_charge; ++idx) {
            _lines.emplace_back(
                _make_line(get_begin(charge->get_coord(), idx), false, sz));
        }
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
                {
                    const auto guard = context_guard(cr);
                    cr->set_line_width(line_width);
                    _make_line(coord, true, sz)->draw(cr);
                    _make_line(coord, false, sz)->draw(cr);
                }
                arr.draw(cr, fill_arrow);
                continue;
            }

            arr.draw(cr, fill_arrow);
        }
    }
}

void Canvas::_draw_lines(const Cairo::RefPtr<Cairo::Context>& cr)
{
    if (_draw_lines_flag && !_lines.empty()) {
        const auto guard = context_guard(cr);
        cr->set_line_width(line_width);
        for (const auto& line : _lines) {
            line->draw(cr);
        }
    }
}

void Canvas::_draw_charges(const Cairo::RefPtr<Cairo::Context>& ctx)
{
    for (const auto& circle : _circles) {
        circle.draw(ctx);
    }
}

void Canvas::_draw_background(const Cairo::RefPtr<Cairo::Context>& ctx)
{
    const auto guard = context_guard(ctx);
    Gdk::Cairo::set_source_rgba(ctx, bg_color);
    ctx->paint();
}

void Canvas::_draw_grid(const Cairo::RefPtr<Cairo::Context>& ctx)
{
    const auto guard = context_guard(ctx);
    const double delta = 10.0;
    Gtk::Allocation allocation = get_allocation();
    const auto sz = size(allocation.get_width(), allocation.get_height());
    size_t x_count = sz.width / delta;
    size_t y_count = sz.height / delta;
    point coord(0.0, 0.0);

    double min_phi = -0.01;
    double max_phi = 0.01;

    for (size_t x_idx = 0; x_idx < x_count; ++x_idx) {
        for (size_t y_idx = 0; y_idx < y_count; ++y_idx) {
            coord.y += delta;
            const auto phi = _field.get_potential(coord);
            Gdk::RGBA color;
            if (phi > 0) {
                auto op = phi / max_phi;
                if (op > 0.7)
                    op = 0.7;
                color.set_rgba(1.0, 0.0, 0.0, op);
            } else {
                auto op = phi / min_phi;
                if (op > 0.7)
                    op = 0.7;
                color.set_rgba(0.0, 0.0, 1.0, op);
            }
            square sq(coord, color);
            sq.draw(ctx);
        }
        coord.x += delta;
        coord.y = 0.0;
    }
}

bool Canvas::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
    Gtk::Allocation allocation = get_allocation();
    const auto sz = size(allocation.get_width(), allocation.get_height());

    _draw_background(cr);
    _draw_grid(cr);
    _draw_arrows(sz, cr);
    _draw_lines(cr);
    _draw_charges(cr);

    return true;
}

bool Canvas::on_button_press_event(GdkEventButton* event)
{
    if (event->type == GDK_BUTTON_PRESS) {
        const auto coord = point(event->x, event->y);
        if (event->button == 1) {
            auto it = std::find_if(
                _circles.begin(), _circles.end(),
                [&coord](const auto& circle) { return circle.is_hint(coord); });
            if (it != _circles.end()) {
                _selected_circle = &(*it);
            } else {
                _charges.emplace_back(charge::type::positive, coord, 1.0);
                _circles.emplace_back(_charges.get_positive_charges().back());
                _init_lines();
                queue_draw();
            }
        } else if (event->button == 3) {
            _charges.emplace_back(charge::type::negative, coord, -1.0);
            _circles.emplace_back(_charges.get_negative_charges().back());
            _init_lines();
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
        _draw_lines_flag = !_draw_lines_flag;
        queue_draw();
    } else if (event->keyval == GDK_KEY_c) {
        _charges.clear();
        _circles.clear();
        _lines.clear();
        _selected_circle = nullptr;
        queue_draw();
    } else if (event->keyval == GDK_KEY_a) {
        _draw_arrows_flag = !_draw_arrows_flag;
        queue_draw();
    } else if (event->keyval == GDK_KEY_t) {
        _line_type = _line_type == base_line::type::line
                         ? base_line::type::curve
                         : base_line::type::line;
        _init_lines();
        queue_draw();
        std::cout << "Line type has been changed to " << (int)_line_type
                  << std::endl;
    } else if (event->keyval == GDK_KEY_j && _line_delta > 2.0) {
        _line_delta -= 2.;
        _init_lines();
        queue_draw();
        std::cout << "Delta line has been changed to " << _line_delta
                  << std::endl;
    } else if (event->keyval == GDK_KEY_k && _line_delta <= 20.0) {
        _line_delta += 2.;
        _init_lines();
        queue_draw();
        std::cout << "Delta line has been changed to " << _line_delta
                  << std::endl;
    }
    return false;
}

bool Canvas::on_motion_notify_event(GdkEventMotion* event)
{
    const auto coord = point(event->x, event->y);
    if (_selected_circle) {
        _selected_circle->move(coord);
        _init_lines();
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
