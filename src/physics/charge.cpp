#include "charge.hpp"
#include "constants.hpp"
#include <cmath>
#include <iostream>
namespace elfield
{

charge::charge(const point& coord, double value)
    : _coord(coord), _value(value)
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
    std::cout << " angle=" << angle * 57.32 << std::endl;
    _lines.insert(angle);
}

std::optional<point> charge::get_next_line_begin()
{
    if (_lines.size() >= get_lines_count()) {
      return {};
    }
    const auto get_delta = [&](){
        const auto size = _lines.size();
        if (size == 0U) {
            return 2 * M_PI / get_lines_count();
        } else if (_lines.size() == 1U) {
            return (2*M_PI - *(_lines.begin())) / (get_lines_count() - size);
        }
        return (2 * M_PI - (*(_lines.rbegin()) - *(_lines.begin()))) / (get_lines_count() - size);
    };
    angle_t phi = (_lines.empty() ? 0.0 : *_lines.rbegin()) + get_delta();
    _lines.insert(phi);
    return point(_coord.x + std::cos(phi) * line_delta,
                     _coord.y + std::sin(phi) * line_delta);
}

size_t charge::get_lines_count() const { return abs(_value) * 8U; }

void charge::reinit_lines()
{
    _lines.clear();
}

} // namespace elfield
