#include "charge.hpp"

namespace maxwell
{

charge::charge(const point& coord, double value) : _coord(coord), _value(value) {}

const point& charge::get_coord() const
{
    return _coord;
}

double charge::get_value() const
{
    return _value;
}

} // namespace maxwell
