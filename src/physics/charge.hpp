#pragma once

#include "graphics/point.hpp"
#include <memory>

namespace maxwell
{

class charge
{
  public:
    enum class type { negative = 0, positive = 1 , any = 2};

    charge(const point& coordinates_, double value_);

    const point& get_coord() const;
    void set_coord(const point& coord);
    double get_value() const;
  private:
    point _coord;
    double _value;
};

using charge_ptr = std::shared_ptr<charge>;

} // namespace maxwell
