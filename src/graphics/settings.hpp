#pragma once
#include "constants.hpp"
#include <gdkmm/rgba.h>

namespace elfield
{
struct settings {
    Gdk::RGBA background_color = bg_color;
};
} // namespace elfield
