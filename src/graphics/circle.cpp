#include "circle.hpp"
#include "constants.hpp"
#include "context_guard.hpp"

#include <gtkmm.h>
namespace maxwell
{

circle::circle(charge& chrg) : _charge(chrg)
{
}

void circle::move(const point& coord)
{
    _charge.get().set_coord(coord);
}

void circle::draw(const Cairo::RefPtr<Cairo::Context>& ctx)
{
    const context_guard guard(ctx);
    if (_charge.get().get_value() >= 0) {
        Gdk::Cairo::set_source_rgba(ctx, positive_charge_color);
    } else {
        Gdk::Cairo::set_source_rgba(ctx, negative_charge_color);
    }

    const auto& coord = _charge.get().get_coord();
    ctx->arc(coord.x, coord.y, charge_radius, 0.0f, 2 * M_PI);
    ctx->fill();
}

void circle::select()
{
    _selected = true;
}

void circle::unselect()
{
    _selected = false;
}

bool circle::is_hint(const point& coord)
{
    const auto delta = _charge.get().get_coord() - coord;
    return delta.module() <= charge_radius;
}

} // namespace maxwell

