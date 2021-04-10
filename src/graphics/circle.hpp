#pragma once
#include "graphics/point.hpp"

namespace maxwell
{

class circle
{
  public:
    explicit circle(const point& coord);
    const point& get_coord() const;
    void set_coord(const point& coord);
    void draw();
    void select();
    void unselect();
  private:
    charge _charge;
    bool _selected;
};

}
