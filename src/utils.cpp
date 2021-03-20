#include "utils.hpp"

#include <math.h>

namespace maxwell
{

angle_t get_angle(double cos_val, double sin_val)
{
    auto angle = acos(cos_val);
    return sin_val < 0.0 ? -1 * angle : angle;
}

} // namespace maxwell
