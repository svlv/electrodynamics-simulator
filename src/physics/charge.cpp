#include "charge.hpp"
#include <cmath>
#include <iostream>
namespace elfield
{

charge::charge(const point& coord, double value)
    : _coord(coord), _value(value), _lines(get_lines_count(), 0U)
{
}

charge::charge(const charge& chrg) : _coord(chrg._coord), _value(chrg._value) {}

const point& charge::get_coord() const { return _coord; }

point& charge::get_coord() { return _coord; }
double charge::get_value() const { return _value; }
double& charge::get_value() { return _value; }

void charge::set_coord(const point& coord) { _coord = coord; }
void charge::set_value(double value) { _value = value; }

void charge::add_line(angle_t angle)
{
    const size_t idx = angle / (2 * M_PI / get_lines_count());
    std::cout << "idx=" << idx << " angle=" << angle * 57.32 << std::endl;
    ++_lines[idx];
}

size_t charge::get_lines_count(size_t idx) const
{
    if (idx < _lines.size()) {
        return _lines[idx];
    }
    return 0U;
}

size_t charge::get_lines_count() const { return abs(_value) * 8U; }

void charge::reinit_lines()
{
    _lines.clear();
    _lines.resize(get_lines_count());
}

} // namespace elfield
