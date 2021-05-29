#include "window.hpp"
#include <iostream>

namespace elfield
{

main_window::main_window() : _box(Gtk::Orientation::ORIENTATION_VERTICAL)
{
    set_title("Elfield");
    set_size_request(200, 200);
    set_border_width(10);

    _box.pack_start(_canvas);
    _box.pack_start(_cli, Gtk::PACK_SHRINK);

    add(_box);

    _canvas.property_visible() = true;

    _cli.hide();
    _box.show();
}

bool main_window::on_key_press_event(GdkEventKey* event)
{
    if (event->keyval == GDK_KEY_q && !_cli.has_focus()) {
        close();
        return true;
    } else if (event->keyval == GDK_KEY_colon) {
        _cli.show();
        _cli.grab_focus();
    } else if (event->keyval == GDK_KEY_Escape) {
        _cli.hide();
        _canvas.grab_focus();
    }
    return Gtk::Window::on_key_press_event(event);
}

} // namespace elfield
