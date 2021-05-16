#include "graphics/base_line.hpp"

namespace elfield
{

base_line::base_line(const point& begin) : _points{begin} {}

base_line::base_line(const point& begin, color_t color)
    : _points{begin}, _color(color)
{
}

void base_line::add_point(const point& coord) { _points.push_back(coord); }

void base_line::add_chevron(const chevron& chvrn)
{
    _chevrons.push_back(chvrn);
}

void base_line::draw_chevrons(const Cairo::RefPtr<Cairo::Context>& ctx) const
{
    for (const auto& chvrn : _chevrons) {
        chvrn.draw(ctx);
    }
}

void base_line::fill() {}

} // namespace elfield
