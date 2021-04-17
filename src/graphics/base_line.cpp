#include "graphics/base_line.hpp"

namespace maxwell
{

base_line::base_line(const point& coord) : _points{coord} {}

void base_line::add_point(const point& coord)
{
  _points.push_back(coord);
}
void base_line::fill()
{
}

} // namespace maxwell

