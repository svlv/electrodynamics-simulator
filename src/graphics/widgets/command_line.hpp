#pragma once

#include <gtkmm/entry.h>

namespace elfield
{
class command_line : public Gtk::Entry
{
  public:
    command_line() = default;
};
} // namespace elfield
