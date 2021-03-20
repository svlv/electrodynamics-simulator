#pragma once

#include "graphics/point.hpp"

namespace maxwell
{

class charge
{
  public:
    enum class type { negative = 0, positive = 1 };

    charge(const point& coordinates_, double value_);

    const point& get_coord() const;
    double get_value() const;

  private:
    point _coord;
    double _value;
};

} // namespace maxwell
