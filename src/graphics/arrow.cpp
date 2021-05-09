#include "arrow.hpp"
#include <iostream>

namespace elfield
{

arrow::arrow(const params& size, const point& coord, angle_t angle)
    : primitive(coord, angle, _init(size)), _is_selected(false)
{
}

std::vector<point> arrow::_init(const params& _size) const
{
    return {{(_size.b + _size.d) / -2.0, _size.a / -2.0},
            {(_size.b + _size.d) / -2.0, _size.a / 2.0},
            {(_size.b - _size.d) / 2.0, _size.a / 2.0},
            {(_size.b - _size.d) / 2.0, _size.a / 2.0 + _size.c},
            {(_size.b + _size.d) / 2.0, 0},
            {(_size.b - _size.d) / 2.0, (-1) * (_size.a / 2 + _size.c)},
            {(_size.b - _size.d) / 2.0, _size.a / -2.0},
            {(_size.b + _size.d) / -2.0, _size.a / -2.0}};
}

void arrow::draw(const Cairo::RefPtr<Cairo::Context>& context, bool fill) const
{
    if (_points.empty()) {
        return;
    }

    context->move_to(_points[0].x, _points[0].y);

    for (size_t idx = 1; idx < _points.size(); ++idx) {
        context->line_to(_points[idx].x, _points[idx].y);
    }

    if (fill) {
        context->fill();
    }

    context->stroke();
}

bool arrow::is_hint(const point& coord)
{
    const auto check = [&coord, this](size_t i1, size_t i2) {
        return (_points[i2].x - _points[i1].x) * (coord.y - _points[i1].y) -
                   (_points[i2].y - _points[i1].y) * (coord.x - _points[i1].x) <
               0;
    };

    const auto inside_triangle = [&check]() {
        return check(5, 3) && check(3, 4) && check(4, 5);
    };

    const auto inside_rectangle = [&check]() {
        return check(0, 1) && check(1, 2) && check(2, 6) && check(6, 0);
    };

    return inside_triangle() || inside_rectangle();
}

void arrow::select(bool flag) { _is_selected = flag; }

bool arrow::is_selected() const { return _is_selected; }

const point& arrow::get_coord() const { return _coord; }

} // namespace elfield
