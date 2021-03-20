#include "arrow.hpp"
#include <iostream>

namespace maxwell
{

arrow::arrow(const params& size, const point& coord, angle_t angle) : _size(size), _coord(0.0, 0.0), _angle(0.0)
{
    _init();
    rotate(angle);
    move(coord);
}

void arrow::_init()
{
    _points = {
        {(_size.b + _size.d) / -2.0, _size.a / -2.0},
        {(_size.b + _size.d) / -2.0, _size.a / 2.0},
        {(_size.b - _size.d) / 2.0 , _size.a / 2.0},
        {(_size.b - _size.d)/2.0, _size.a/2.0 + _size.c},
        {(_size.b + _size.d) / 2.0, 0},
        {(_size.b - _size.d) / 2.0, (-1)*(_size.a/2 + _size.c)},
        {(_size.b - _size.d) / 2.0, _size.a / -2.0},
        {(_size.b + _size.d) / -2.0, _size.a / -2.0}
    };
}

void arrow::rotate(angle_t angle)
{
    const auto delta = angle - _angle;
    for (auto& point : _points)
    {
        point.rotate(delta);
    }
    _angle = angle;
}

void arrow::move(const point& coord)
{
    const auto delta = coord - _coord;
    for (auto& point : _points)
    {
        point.move(delta);
    }
    _coord = coord;
}

void arrow::draw(const Cairo::RefPtr<Cairo::Context>& context) const
{   
    if (_points.empty())
    {
        return;
    }

    context->move_to(_points[0].x, _points[0].y);
    
    for (size_t idx = 1; idx < _points.size(); ++idx) {
        context->line_to(_points[idx].x, _points[idx].y);
    }

    context->stroke();
}

} // namespace maxwell
