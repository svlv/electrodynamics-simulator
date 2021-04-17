#pragma once

#include "graphics/point.hpp"
#include <vector>
#include <cairomm/context.h>

namespace maxwell
{
class base_line
{
  public:
    explicit base_line(const point& begin);
    base_line() = default;
    void add_point(const point& coord);
    virtual void fill();
    virtual void draw(const Cairo::RefPtr<Cairo::Context>& ctx) const = 0;
  protected:
    std::vector<point> _points;
};
} // namespace maxwell

