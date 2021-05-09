#include "graphics/chevron.hpp"
#include "graphics/point.hpp"
#include <cmath>
#include <iostream>

namespace elfield
{

chevron::chevron(const params& prms, const point& coord, angle_t angle)
    : primitive(coord, angle, _init(prms))
{
}

std::vector<point> chevron::_init(const params& prms) const
{
    double a = std::cos(prms.angle) * prms.d;
    double b = std::sin(prms.angle) * prms.d;
    return {{-1.0 * a, -1.0 * b}, {0.0, 0.0}, {-1.0 * a, b}};
}

void chevron::draw(const Cairo::RefPtr<Cairo::Context>& context) const
{
    if (_points.empty()) {
        return;
    }

    context->move_to(_points[0].x, _points[0].y);

    for (size_t idx = 1; idx < _points.size(); ++idx) {
        context->line_to(_points[idx].x, _points[idx].y);
    }

    context->stroke();
}

} // namespace elfield
