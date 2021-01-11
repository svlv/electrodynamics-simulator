#pragma once

namespace maxwell
{

struct Coordinates
{
    Coordinates(double x_, double y_) : x(x_), y(y_) {}
    Coordinates(const Coordinates& other) : x(other.x), y(other.y) {}
    double x;
    double y;
};

} // namespace maxwell
