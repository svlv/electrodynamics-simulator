#pragma once

#include "physics/charge.hpp"
#include <cairomm/context.h>

namespace maxwell
{

class circle
{
  public:
    explicit circle(const charge_ptr& chrg);

    void move(const point& coord);
    void draw(const Cairo::RefPtr<Cairo::Context>& ctx) const;

    bool is_hint(const point& coord) const;

    void select(bool val);
    bool is_selected() const;

    charge_ptr get_charge() const;

  private:
    charge_ptr _charge;
    bool _selected;
};

} // namespace maxwell
