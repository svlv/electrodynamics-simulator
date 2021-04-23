#include "graphics/square.hpp"
#include "graphics/context_guard.hpp"

#include <gtkmm.h>

namespace maxwell
{

double square::size = 10.0;

square::square(const point& coord) : _coord(coord) {}

square::square(const point& coord, const Gdk::RGBA& color)
    : _coord(coord), _color(color)
{
}

const point& square::get_coord() const { return _coord; }

void square::set_color(const Gdk::RGBA& color) { _color = color; }

void square::draw(const Cairo::RefPtr<Cairo::Context>& ctx) const
{
    const auto guard = context_guard(ctx);
    Gdk::Cairo::set_source_rgba(ctx, _color);
    ctx->rectangle(_coord.x, _coord.y, size, size);
    ctx->fill();
}

} // namespace maxwell
