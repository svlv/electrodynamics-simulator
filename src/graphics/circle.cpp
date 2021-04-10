#include "circle.hpp"

namespace maxwell
{

circle::circle(const charge& chrg) : _charge(chrg) {}

const charge& circle::get_charge() const
{
  return _charge;
}

void circle::move(const point& coord)
{
  _charge.set_coord(coord);
}

void circle::draw()
{
}

void circle::select()
{
  _selected = true;
}

void circle::unselect()
{
  _selected = false;
}

}

