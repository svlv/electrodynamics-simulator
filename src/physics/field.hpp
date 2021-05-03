#pragma once

#include "graphics/point.hpp"
#include "physics/charges.hpp"
#include <functional>

namespace elfield
{

class field
{
  public:
    explicit field(const charges& chrgs);

    double get_angle(const point& coord) const;

    double get_Ex(const point& coord) const;
    double get_Ey(const point& coord) const;
    double get_E(const point& coord) const;

    double get_cos(const point& coord) const;
    double get_sin(const point& coord) const;

    double get_potential(const point& coord) const;

  private:
    std::reference_wrapper<const charges> _charges;
};

} // namespace elfield
