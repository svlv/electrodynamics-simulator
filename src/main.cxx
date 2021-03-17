#include <iostream>

#include <gtkmm.h>

#include "Charge.hxx"
#include "Canvas.hxx"

using namespace maxwell;


class MainWindow : public Gtk::Window
{
public:
    MainWindow();
    virtual ~MainWindow() = default;
protected:
    Canvas _canvas;
    Gtk::Frame _frame;
private:
    bool on_key_press_event(GdkEventKey* event) override;
};

MainWindow::MainWindow()
{
    set_title("Electrodynamics Simulator");
    set_size_request(200, 200);
    set_border_width(10);

    //add_events(Gdk::KEY_PRESS_MASK);

    add(_frame);
    _frame.add(_canvas);

    _frame.property_visible() = true;
    _canvas.property_visible() = true;
    show_all_children();

}

bool MainWindow::on_key_press_event(GdkEventKey* event)
{
    if (event->keyval == GDK_KEY_Escape)
    {
        close();
        return true;
    }
    return Gtk::Window::on_key_press_event(event);
}

int main(int argc, char *argv[])
{
  auto app = Gtk::Application::create(argc, argv, "org.gtkmm.electrodynamics-simulator");

  MainWindow window;

  return app->run(window);
}
