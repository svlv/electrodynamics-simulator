#include "window.hpp"

namespace maxwell
{

MainWindow::MainWindow()
{
    set_title("Elfield");
    set_size_request(200, 200);
    set_border_width(10);

    // add_events(Gdk::KEY_PRESS_MASK);

    add(_frame);
    _frame.add(_canvas);

    _frame.property_visible() = true;
    _canvas.property_visible() = true;
    show_all_children();
}

bool MainWindow::on_key_press_event(GdkEventKey* event)
{
    if (event->keyval == GDK_KEY_q) {
        close();
        return true;
    }
    return Gtk::Window::on_key_press_event(event);
}

} // namespace maxwell
