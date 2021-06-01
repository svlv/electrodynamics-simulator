#pragma once

#include <gdkmm/rgba.h>
#include <optional>

namespace elfield
{

using angle_t = double;
using color_t = Gdk::RGBA;
using color_opt = std::optional<color_t>;

} // namespace elfield
