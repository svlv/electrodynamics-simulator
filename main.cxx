#include <iostream>

#include <gtkmm.h>

#include <gtkmm/drawingarea.h>

#include <optional>
#include <numeric>

struct Coordinates
{
    Coordinates(int x_, int y_) : x(x_), y(y_) {}
    int x;
    int y;
};

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

    enum class ChargeType
    {
        Negative = 0,
        Positive = 1
    };


    using Data = std::vector<Charge>;
    using Iter = Data::iterator;
    using ConstIter = Data::const_iterator;

    template <typename... Ts>
    void emplaceBackPositiveCharge(Ts&&... args)
    {
        _positiveCharges.emplace_back(std::forward<Ts>(args)...);
    }

    template <typename... Ts>
    void emplaceBackNegativeCharge(Ts&&... args)
    {
        _negativeCharges.emplace_back(std::forward<Ts>(args)...);
    }

    void clear()
    {
        _positiveCharges.clear();
        _negativeCharges.clear();
    }

    double getEx(int x, int y) const
    {
        auto sumEx = [x, y](double Ex, const Charge& charge) -> double
        {
            int dx = x - charge.x;
            int dy = y - charge.y;
            return Ex + charge.value * dx / pow(pow(dx, 2.0) + pow(dy, 2.0), 1.5);
        };

        return
          std::accumulate(_positiveCharges.cbegin(), _positiveCharges.cend(), 0.0, sumEx) +
          std::accumulate(_negativeCharges.cbegin(), _negativeCharges.cend(), 0.0, sumEx);
    }

    double getEy(int x, int y) const
    {
        auto sumEy = [x, y](double Ey, const Charge& charge) -> double
        {
            int dx = x - charge.x;
            int dy = y - charge.y;
            return Ey + charge.value * dy / pow(pow(dx, 2.0) + pow(dy, 2.0), 1.5);
        };

        return
          std::accumulate(_positiveCharges.cbegin(), _positiveCharges.cend(), 0.0, sumEy) +
          std::accumulate(_negativeCharges.cbegin(), _negativeCharges.cend(), 0.0, sumEy);
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

    std::optional<Coordinates> isComeToNegative(int x, int y) const
    {
        return isNear(x, y, ChargeType::Negative);
    }

    std::optional<Coordinates> isComeToPositive(int x, int y) const
    {
        return isNear(x, y, ChargeType::Positive);
    }

    const Data& getPositiveCharges() const { return _positiveCharges; }
    const Data& getNegativeCharges() const { return _negativeCharges; }

private:
    std::optional<Coordinates> isNear(int x, int y, ChargeType type) const
    {
        auto pred = [x, y](const Charge& charge)
        {
            const double delta = 100;
            return pow(charge.x - x, 2.0) + pow(charge.y - y, 2.0) < delta;
        };
        switch (type)
        {
            case ChargeType::Negative:
            {
                auto it = std::find_if(_negativeCharges.cbegin(), _negativeCharges.cend(), pred);
                if (it != _negativeCharges.cend())
                {
                    return Coordinates(it->x, it->y);
                }
                break;
            }
            case ChargeType::Positive:
            {
                auto it = std::find_if(_positiveCharges.cbegin(), _positiveCharges.cend(), pred);
                if (it != _positiveCharges.cend())
                {
                    return Coordinates(it->x, it->y);
                }
                break;
            }
      }
      return {};
    }

    Data _positiveCharges;
    Data _negativeCharges;
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

    // draw lines
    cr->save();
    Gdk::RGBA lines_color("#872323");
    cr->set_line_width(5.0);

    const size_t lines_count = 8;

    const auto& posCharges = _charges.getPositiveCharges();
    const auto& negCharges = _charges.getNegativeCharges();

    if (_draw_lines)
    {
        const double dl = 10.0;
        for (const auto& charge : posCharges)
        {
            for (size_t i = 0; i < lines_count; ++i)
            {
                cr->move_to(charge.x, charge.y);
                double x = charge.x + cos(i * 2 * M_PI / lines_count) * dl;
                double y = charge.y + sin(i * 2 * M_PI / lines_count) * dl;
                for (size_t j = 0; x < width && x > 0 && y < height && y > 0 && j < 1000; ++j)
                {
                    auto end = _charges.isComeToNegative(x, y);
                    if (end)
                    {
                        cr->line_to(end->x, end->y);
                        break;
                    }
                    cr->line_to(x,y);
                    double dx = _charges.getCos(x, y) * dl;
                    double dy = _charges.getSin(x, y) * dl;
                    if (fabs(dx) < 1.0 && fabs(dy) < 1.0)
                    {
                      break;
                    }
                    x += dx;
                    y += dy;
                }
                cr->stroke();
            }
        }

        for (const auto& charge : negCharges)
        {
            for (size_t i = 0; i < lines_count; ++i)
            {
                cr->move_to(charge.x, charge.y);
                double x = charge.x + cos(i * 2 * M_PI / lines_count) * dl;
                double y = charge.y + sin(i * 2 * M_PI / lines_count) * dl;
                for (size_t j = 0; x < width && x > 0 && y < height && y > 0 && j < 1000; ++j)
                {
                    auto end = _charges.isComeToPositive(x, y);
                    if (end)
                    {
                        cr->line_to(end->x, end->y);
                        break;
                    }
                    cr->line_to(x,y);
                    double dx = _charges.getCos(x, y) * dl;
                    double dy = _charges.getSin(x, y) * dl;
                    if (fabs(dx) < 1.0 && fabs(dy) < 1.0)
                    {
                      break;
                    }
                    x -= dx;
                    y -= dy;
                }
                cr->stroke();
            }
        }
    }
    cr->restore();

    // draw charges
    cr->save();

    auto drawArc = [&cr](const Charges::Charge& charge)
    {
        cr->arc(charge.x, charge.y, 10.0f, 0.0f, 2 * M_PI);
        cr->fill();
    };

    Gdk::RGBA positive_charge_color("#ff0000");
    Gdk::Cairo::set_source_rgba(cr, positive_charge_color);
    std::for_each(posCharges.cbegin(), posCharges.cend(), drawArc);

    Gdk::RGBA negative_charge_color("#0000ff");
    Gdk::Cairo::set_source_rgba(cr, negative_charge_color);
    std::for_each(negCharges.cbegin(), negCharges.cend(), drawArc);

    cr->restore();

    return true;
}

bool Canvas::on_button_press_event(GdkEventButton* event)
{
    if (event->type == GDK_BUTTON_PRESS)
    {
        if (event->button == 1)
        {
            _charges.emplaceBackPositiveCharge(event->x, event->y, 1);
            queue_draw();
        }
        else if (event->button == 3)
        {
            _charges.emplaceBackNegativeCharge(event->x, event->y, -1);
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

