#include "point.hpp"
#include <cmath>

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

void point::move(const point& coord) { this->operator+=(coord); }

void point::rotate(angle_t angle)
{
    const auto cos_val = std::cos(angle);
    const auto sin_val = std::sin(angle);
    const auto x_ = cos_val * x - sin_val * y;
    const auto y_ = sin_val * x + cos_val * y;
    x = x_;
    y = y_;
}

point operator+(const point& lhs, const point& rhs)
{
    return point(lhs.x + rhs.x, lhs.y + rhs.y);
}

point operator-(const point& lhs, const point& rhs)
{
    return point(lhs.x - rhs.x, lhs.y - rhs.y);
}

bool operator<(const point& lhs, const point& rhs)
{
  return lhs.x < rhs.x;
}

double point::module() const { return pow(pow(x, 2.0) + pow(y, 2.0), 0.5); }

} // namespace maxwell
