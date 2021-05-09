#pragma once

#include "graphics/point.hpp"
#include "types.hpp"
#include <cairomm/context.h>
#include <vector>

namespace elfield
{

class primitive
{
  public:
    primitive(const point& coord, angle_t angle, std::vector<point>&& points);
    virtual ~primitive() = default;
    void rotate(angle_t angle);
    void move(const point& coord);
    virtual void draw(const Cairo::RefPtr<Cairo::Context>& context) const = 0;
  protected:
    point _coord;
    angle_t _angle;
    std::vector<point> _points;
};

} // namespace elfield

