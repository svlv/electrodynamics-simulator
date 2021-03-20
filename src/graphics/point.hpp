#pragma once

namespace maxwell
{

struct point
{
    point(double x_, double y_);
    point(const point& other);
    point& operator+=(const point& coord);
    point& operator-=(const point& coord);
    double x;
    double y;
};

} //namespace maxwell
