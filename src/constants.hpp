#pragma once

#include "graphics/arrow.hpp"
#include "graphics/chevron.hpp"
#include <gdkmm/rgba.h>

namespace elfield
{

// Background color
const Gdk::RGBA bg_color("#ffefd7");
// Lines color
const Gdk::RGBA lines_color("#872323");
// Positive charge color
const Gdk::RGBA positive_charge_color("#ff0000");
// Color for highlighted positive charges
const Gdk::RGBA positive_charge_color_selected("#780000");
// Negative charge color
const Gdk::RGBA negative_charge_color("#0000ff");
// Color for highlighted negative charges
const Gdk::RGBA negative_charge_color_selected("#000078");
// Arrow color
const Gdk::RGBA arrow_color("#004156");
// Color for hihglighed arrows
const Gdk::RGBA highlight_arrow_color("#2398AB");

const double line_delta = 10.0;
const double arrow_delta = 50.0;
const arrow::params default_arrow_size = {20.0, 28.0, 10.0, 20.0};
const chevron::params default_chevron_size = {10.0, 0.52359};
const double total_chevron_step = 50.0;
const bool fill_arrow = true;
const size_t lines_per_charge = 8;
const double line_width = 3.0;

const double charge_radius = 10.0;
} // namespace elfield
