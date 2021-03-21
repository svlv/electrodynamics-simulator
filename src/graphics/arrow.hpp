#pragma once

#include "graphics/point.hpp"
#include "types.hpp"

#include "point.hpp"
#include <vector>

#include <cairomm/context.h>

namespace maxwell
{

class arrow
{
  public:
    struct params {
        double a;
        double b;
        double c;
        double d;
    };

    arrow(const params& size, const point& coord, angle_t angle);
    arrow() = default;

    void rotate(angle_t angle);
    void move(const point& coord);
    void draw(const Cairo::RefPtr<Cairo::Context>& context) const;
    bool is_hint(const point& coord);

    void select(bool flag);
    bool is_selected() const;

    const point& get_coord() const;

  private:
    void _init();

    params _size;
    point _coord;
    angle_t _angle;
    std::vector<point> _points;
    bool _is_selected;
};

} // namespace maxwell
