#pragma once

#include <cairomm/context.h>

namespace maxwell
{

// TODO: move in separate file
// TODO: rename point
// TODO: add rotate method
struct Coordinates
{
    Coordinates(double x_, double y_) : x(x_), y(y_) {}
    Coordinates(const Coordinates& other) : x(other.x), y(other.y) {}
    Coordinates& operator+=(const Coordinates& coord)
    {
        x += coord.x;
        y += coord.y;
        return *this;
    }
    Coordinates& operator-=(const Coordinates& coord)
    {
        x -= coord.x;
        y -= coord.y;
        return *this;
    }
    double x;
    double y;
};

struct ArrowSize
{
    double a;
    double b;
    double c;
    double d;
};

static const ArrowSize DefaultArrowSize = {20.0, 28.0, 10.0, 20.0};

void draw_arrow(const Cairo::RefPtr<Cairo::Context>& cr,
                Coordinates coord, float sin_, float cos_, const ArrowSize& size = DefaultArrowSize);

void rotate(Coordinates& coord, float sin_, float cos_);

} // namespace maxwell
