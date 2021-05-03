#pragma once

#include "types.hpp"

namespace elfield
{

struct size {
    size(double w, double h) : width(w), height(h) {}
    double width;
    double height;
};

angle_t get_angle(double cos_val, double sin_val);

} // namespace elfield
