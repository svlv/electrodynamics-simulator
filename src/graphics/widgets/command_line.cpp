#include "graphics/widgets/command_line.hpp"
#include "graphics/canvas.hpp"
#include <gdkmm/rgba.h>
#include <iostream>

namespace elfield
{

class parser
{
  public:
    parser() = default;

  private:
};

command_line::command_line(canvas& cnvs) : _canvas(cnvs)
{
    add_events(Gdk::KEY_PRESS_MASK);
}

bool command_line::on_key_press_event(GdkEventKey* event)
{
    if (event->keyval == GDK_KEY_Return) {
        std::istringstream ss(get_text());
        std::string func_name;
        ss >> func_name;
        if (func_name == ":set_background_color") {
            std::string color;
            ss >> color;
            Gdk::RGBA rgba(color);
            _canvas.set_background_color(rgba);
        }
        _hist.add(get_text());
        set_text(":");
        set_position(1);
        return true;
    } else if (event->keyval == GDK_KEY_Up) {
        const auto& line = _hist.prev();
        set_text(line);
        set_position(line.size());
        return true;
    }
    return Gtk::Entry::on_key_press_event(event);
}

void command_line::on_delete_text(int start_pos, int end_pos)
{
    if (start_pos != 0 || end_pos == -1) {
        Gtk::Entry::on_delete_text(start_pos, end_pos);
    } else if (start_pos == 0 && get_text().size() == end_pos) {
        Gtk::Entry::on_delete_text(start_pos, end_pos);
        hide();
        _canvas.grab_focus();
    }
}
} // namespace elfield
