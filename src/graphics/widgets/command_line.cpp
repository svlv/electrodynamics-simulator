#include "graphics/widgets/command_line.hpp"
#include "graphics/canvas.hpp"

namespace elfield
{
command_line::command_line(canvas& cnvs) : _canvas(cnvs)
{
    add_events(Gdk::KEY_PRESS_MASK);
}

bool command_line::on_key_press_event(GdkEventKey* event)
{
    if (event->keyval == GDK_KEY_Return) {
        set_text(":");
        return true;
    }
    return Gtk::Entry::on_key_press_event(event);
}

} // namespace elfield
