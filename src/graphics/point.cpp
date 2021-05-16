#include "point.hpp"
#include <cmath>
#include <iostream>
namespace elfield
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

bool operator<(const point& lhs, const point& rhs) { return lhs.x < rhs.x; }

double point::module() const { return pow(pow(x, 2.0) + pow(y, 2.0), 0.5); }

angle_t point::get_phi(const point& coord) const
{
    const auto is_null = [](const double val) {
        return val < 1E-3 && val > -1E-3;
    };
    auto p = coord - *this;
    std::cout << p.x << " " << p.y << std::endl;
    p.y *= (-1.);
    if (is_null(p.x)) {
        return p.y > 0.0 ? M_PI / 2.0 : 3.0 * M_PI / 2.0;
    } else if (p.x > 0.0) {
        return p.y >= 0.0 ? std::atan(p.y / p.x)
                          : std::atan(p.y / p.x) + 2 * M_PI;
    }
    return std::atan(p.y / p.x) + M_PI;
}

} // namespace elfield
