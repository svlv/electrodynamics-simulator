#pragma once

#include "graphics/point.hpp"
#include <memory>
#include <set>
#include <optional>

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

  private:
    point _coord;
    double _value;
};

using charge_ptr = std::shared_ptr<charge>;

} // namespace elfield
