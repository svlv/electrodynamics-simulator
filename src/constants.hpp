#pragma once

#include "graphics/arrow.hpp"
#include <gdkmm/rgba.h>

namespace elfield
{

const Gdk::RGBA bg_color("#ffefd7");
const Gdk::RGBA lines_color("#872323");
const Gdk::RGBA positive_charge_color("#ff0000");
const Gdk::RGBA positive_charge_color_selected("#780000");
const Gdk::RGBA negative_charge_color("#0000ff");
const Gdk::RGBA negative_charge_color_selected("#000078");

const Gdk::RGBA arrow_color("#004156");
const Gdk::RGBA highlight_arrow_color("#2398AB");

const double line_delta = 10.0;
const double arrow_delta = 50.0;
const arrow::params default_arrow_size = {20.0, 28.0, 10.0, 20.0};
const bool fill_arrow = true;
const size_t lines_per_charge = 8;
const double line_width = 5.0;

const double charge_radius = 10.0;
} // namespace elfield
