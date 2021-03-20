#include "point.hpp"

namespace maxwell
{

point::point(double x_, double y_) : x(x_), y(y_) {}

point::point(const point& other) : x(other.x), y(other.y) {}

point& point::operator+=(const point& coord)
{
    x += coord.x;
    y += coord.y;
    return *this;
}
point& point::operator-=(const point& coord)
{
    x -= coord.x;
    y -= coord.y;
    return *this;
}

} // namespace maxwell
