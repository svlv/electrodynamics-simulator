#include "circle.hpp"
#include "constants.hpp"
#include "context_guard.hpp"

#include <gtkmm.h>
namespace maxwell
{

circle::circle(const std::shared_ptr<charge>& chrg) : _charge(chrg) {}

void circle::move(const point& coord) { _charge->set_coord(coord); }

void circle::draw(const Cairo::RefPtr<Cairo::Context>& ctx) const
{
    const context_guard guard(ctx);
    const auto& coord = _charge->get_coord();
    if (_charge->get_value() >= 0) {
        _selected
            ? Gdk::Cairo::set_source_rgba(ctx, positive_charge_color_selected)
            : Gdk::Cairo::set_source_rgba(ctx, positive_charge_color);
    } else {
        _selected
            ? Gdk::Cairo::set_source_rgba(ctx, negative_charge_color_selected)
            : Gdk::Cairo::set_source_rgba(ctx, negative_charge_color);
    }
    ctx->arc(coord.x, coord.y, charge_radius, 0.0f, 2 * M_PI);
    ctx->fill();
}

void circle::select(bool val) { _selected = val; }

bool circle::is_selected() const { return _selected; }

bool circle::is_hint(const point& coord) const
{
    const auto delta = _charge->get_coord() - coord;
    return delta.module() <= charge_radius;
}

charge_ptr circle::get_charge() const { return _charge; }

} // namespace maxwell
