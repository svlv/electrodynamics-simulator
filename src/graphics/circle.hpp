#pragma once

#include "physics/charge.hpp"
#include <cairomm/context.h>

namespace maxwell
{

class circle
{
public:
    explicit circle(charge& chrg);

    void move(const point& coord);
    void draw(const Cairo::RefPtr<Cairo::Context>& ctx) const;

    bool is_hint(const point& coord) const;

    void select(bool val);

private:
    std::reference_wrapper<charge> _charge;
    bool _selected;
};

} // namespace maxwell

