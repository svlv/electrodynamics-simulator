#include "graphics/primitive.hpp"
namespace elfield
{

primitive::primitive(const point& coord, angle_t angle,
                     std::vector<point>&& points)
    : _angle(0.0), _coord(0.0, 0.0), _points(std::move(points))
{
    rotate(angle);
    move(coord);
}

void primitive::rotate(angle_t angle)
{
    const auto coord = _coord;
    move(point(0.0, 0.0));

    const auto delta = angle - _angle;
    for (auto& point : _points) {
        point.rotate(delta);
    }
    _angle = angle;

    move(coord);
}

void primitive::move(const point& coord)
{
    const auto delta = coord - _coord;
    for (auto& point : _points) {
        point.move(delta);
    }
    _coord = coord;
}
} // namespace elfield
