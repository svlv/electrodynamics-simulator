#pragma once

#include "graphics/point.hpp"
#include "types.hpp"

#include <vector>
#include "point.hpp"

#include <cairomm/context.h>

namespace maxwell
{

class arrow
{
public:

    struct params
    {
        double a;
        double b;
        double c;
        double d;
    };

    arrow(const params& size, const point& coord, angle_t angle);

    void rotate(angle_t angle);
    void move(const point& coord);
    void draw(const Cairo::RefPtr<Cairo::Context>& context) const;

private:
    void _init();

    params _size;
    point _coord;
    angle_t _angle;
    std::vector<point> _points;
};

} // namespace maxwell
