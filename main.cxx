#include <iostream>

#include <gtkmm.h>

#include <gtkmm/drawingarea.h>


class Charges
{
public:

    struct Charge
    {
        Charge(int x_, int y_, int value_)
          : x(x_)
          , y(y_)
          , value(value_) {}
        int x;
        int y;
        int value;
    };

    using Data = std::vector<Charge>;
    using Iter = Data::iterator;
    using ConstIter = Data::const_iterator;

    template <typename... Ts>
    void emplace_back(Ts&&... args)
    {
        _charges.emplace_back(std::forward<Ts>(args)...);
    }

    ConstIter begin() const { return _charges.cbegin(); }
    ConstIter end() const { return _charges.cend(); }
    Iter begin() { return _charges.begin(); }
    Iter end() { return _charges.end(); };

    void clear() { _charges.clear(); }

    double getEx(int x, int y) const
    {
        double Ex = 0.0;
        for (const auto& charge : _charges)
        {
            int dx = x - charge.x;
            int dy = y - charge.y;
            Ex += charge.value * dx / pow(pow(dx, 2.0) + pow(dy, 2.0), 1.5);
        }
        return Ex;
    }

    double getEy(int x, int y) const
    {
        double Ey = 0.0;
        for (const auto& charge : _charges)
        {
            int dx = x - charge.x;
            int dy = y - charge.y;
            Ey += charge.value * dy / pow(pow(dx, 2.0) + pow(dy, 2.0), 1.5);
        }
        return Ey;
    }

    double getE(int x, int y) const
    {
        return pow(pow(getEx(x, y), 2.0) + pow(getEy(x, y), 2.0), 0.5);
    }

    double getCos(int x, int y) const
    {
        return getEx(x, y) / getE(x, y);
    }

    double getSin(int x, int y) const
    {
        return getEy(x, y) / getE(x, y);
    }

    bool nearNegatives(int x, int y) const
    {
        const int d = 100;
        for (const auto& charge : _charges)
        {
            if (charge.value < 0)
            {
                if (pow(charge.x - x, 2.0) + pow(charge.y - y, 2.0) < d)
                {
                  return true;
                }
            }
        }
        return false;
    }

private:

    Data _charges;
};

class Canvas : public Gtk::DrawingArea
{
public:
    Canvas();
    virtual ~Canvas() = default;
protected:
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;

    bool on_button_press_event(GdkEventButton* event) override;
    bool on_key_press_event(GdkEventKey* event) override;
private:
    Charges _charges;
    bool _draw_lines = true;
};

Canvas::Canvas()
{
    add_events(Gdk::BUTTON_PRESS_MASK);
    add_events(Gdk::KEY_PRESS_MASK);
    property_can_focus() = true;
}

bool Canvas::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
    Gtk::Allocation allocation = get_allocation();
    const int width = allocation.get_width();
    const int height = allocation.get_height();

    // draw background
    cr->save();
    Gdk::RGBA bg_color("#ffefd7");
    Gdk::Cairo::set_source_rgba(cr, bg_color);
    cr->paint();
    cr->restore();

    // draw charges
    Gdk::RGBA positive_charge_color("#ff0000");
    Gdk::RGBA negative_charge_color("#0000ff");
    cr->save();
    for (const auto& charge : _charges)
    {
        if (charge.value > 0)
        {
            Gdk::Cairo::set_source_rgba(cr, positive_charge_color);
        }
        else
        {
            Gdk::Cairo::set_source_rgba(cr, negative_charge_color);
        }
        cr->arc(charge.x, charge.y, 10.0f, 0.0f, 2 * M_PI);
        cr->fill();
    }
    cr->restore();

    cr->save();
    Gdk::RGBA lines_color("#872323");
    cr->set_line_width(5.0);
    if (_draw_lines)
    {
        const double dl = 10.0;
        for (const auto& charge : _charges)
        {
            if (charge.value <= 0)
            {
                continue;
            }
            for (size_t i = 0; i < 8; ++i)
            {
                double x = charge.x + cos(i * M_PI / 4) * dl;
                double y = charge.y + sin(i * M_PI / 4) * dl;
                cr->move_to(x, y);
                for (size_t j = 0; x < width && x > 0 && y < height && y > 0 && !_charges.nearNegatives(x, y) && j < 100; ++j)
                {
                    x += _charges.getCos(x, y) * dl;
                    y += _charges.getSin(x, y) * dl;
                    if (x < width && x > 0 && y < height && y > 0)
                    {
                        cr->line_to(x,y);
                    }
                }
                cr->stroke();
            }
        }
    }
    cr->restore();

    return true;
}

bool Canvas::on_button_press_event(GdkEventButton* event)
{
    if (event->type == GDK_BUTTON_PRESS)
    {
        if (event->button == 1)
        {
            _charges.emplace_back(event->x, event->y, 1);
            queue_draw();
        }
        else if (event->button == 3)
        {
            _charges.emplace_back(event->x, event->y, -1);
            queue_draw();
        }
    }
    std::cout << "Mouse press\n";
    return false;
}

bool Canvas::on_key_press_event(GdkEventKey* event)
{
    if (event->keyval == GDK_KEY_g)
    {
        _draw_lines = !_draw_lines;
        queue_draw();
    }
    else if (event->keyval == GDK_KEY_c)
    {
        _charges.clear();
        queue_draw();
    }
    std::cout << "Canvas : key press\n";
    return false;
}

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
    set_title("Physic Emulator");
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
  auto app = Gtk::Application::create(argc, argv, "org.gtkmm.physics-emulator");

  MainWindow window;

  return app->run(window);
}

