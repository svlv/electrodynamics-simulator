#pragma once

#include <gdkmm/rgba.h>
#include "graphics/arrow.hpp"

namespace maxwell
{

const Gdk::RGBA bg_color("#ffefd7");
const Gdk::RGBA lines_color("#872323");
const Gdk::RGBA positive_charge_color("#ff0000");
const Gdk::RGBA negative_charge_color("#0000ff");

const Gdk::RGBA arrow_color("#004156");
const Gdk::RGBA highlight_arrow_color("#2398AB");

const double arrow_delta = 50.0;
const arrow::params default_arrow_size = {20.0, 28.0, 10.0, 20.0};
const bool fill_arrow = true;

} // namespace maxwell
