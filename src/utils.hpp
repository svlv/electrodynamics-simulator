#pragma once

#include <cairomm/context.h>

#include "graphics/point.hpp"

namespace maxwell
{

struct ArrowSize
{
    double a;
    double b;
    double c;
    double d;
};

static const ArrowSize DefaultArrowSize = {20.0, 28.0, 10.0, 20.0};

void draw_arrow(const Cairo::RefPtr<Cairo::Context>& cr,
                point coord, float sin_, float cos_, const ArrowSize& size = DefaultArrowSize);

void rotate(point& coord, float sin_, float cos_);

} // namespace maxwell
