#pragma once

#include "graphics/point.hpp"
#include <memory>
#include <vector>

namespace elfield
{

class charge
{
  public:
    enum class type { negative = 0, positive = 1, any = 2 };

    charge(const point& coordinates_, double value_);
    charge(const charge& chrg);

    const point& get_coord() const;
    point& get_coord();
    void set_coord(const point& coord);
    double get_value() const;
    double& get_value();
    void set_value(double value);

    void add_line(angle_t angle);
    size_t get_lines_count() const;
    size_t get_lines_count(size_t idx) const;
    void reinit_lines();

  private:
    point _coord;
    double _value;
    std::vector<size_t> _lines;
};

using charge_ptr = std::shared_ptr<charge>;

} // namespace elfield
