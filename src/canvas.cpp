#include "canvas.hpp"

#include <gtkmm.h>

namespace maxwell
{

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

    // draw arrows
    if (!_charges.empty())
    {
        double cur_x = 25;
        double cur_y = 25;
        double delta = 50;

        cr->save();

        while (cur_x + delta / 2.0 < width)
        {
            while (cur_y + delta / 2.0 < height)
            {
                auto coordinates = Coordinates(cur_x, cur_y);
                draw_arrow(cr, coordinates, _charges.getSin(coordinates), _charges.getCos(coordinates));
                cur_y += delta;
            }
            cur_x += delta;
            cur_y = 25;
        }

        cr->restore();
    }

    if (_draw_lines)
    {
        const double dl = 10.0;
        for (const auto& charge : posCharges)
        {
            for (size_t i = 0; i < lines_count; ++i)
            {
                cr->move_to(charge.coordinates.x, charge.coordinates.y);
                double x = charge.coordinates.x + cos(i * 2 * M_PI / lines_count) * dl;
                double y = charge.coordinates.y + sin(i * 2 * M_PI / lines_count) * dl;
                for (size_t j = 0; x < width && x > 0 && y < height && y > 0 && j < 1000; ++j)
                {
                    auto coordinates = Coordinates(x, y);
                    auto end = _charges.isComeToNegative(coordinates);
                    if (end)
                    {
                        cr->line_to(end->x, end->y);
                        break;
                    }
                    cr->line_to(x,y);
                    double dx = _charges.getCos(coordinates) * dl;
                    double dy = _charges.getSin(coordinates) * dl;
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
                cr->move_to(charge.coordinates.x, charge.coordinates.y);
                double x = charge.coordinates.x + cos(i * 2 * M_PI / lines_count) * dl;
                double y = charge.coordinates.y + sin(i * 2 * M_PI / lines_count) * dl;
                for (size_t j = 0; x < width && x > 0 && y < height && y > 0 && j < 1000; ++j)
                {
                    auto coordinates = Coordinates(x, y);
                    auto end = _charges.isComeToPositive(Coordinates(x, y));
                    if (end)
                    {
                        cr->line_to(end->x, end->y);
                        break;
                    }
                    cr->line_to(x,y);
                    double dx = _charges.getCos(coordinates) * dl;
                    double dy = _charges.getSin(coordinates) * dl;
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

    auto drawArc = [&cr](const Charge& charge)
    {
        cr->arc(charge.coordinates.x, charge.coordinates.y, 10.0f, 0.0f, 2 * M_PI);
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
    //std::cout << "Mouse press\n";
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
    //std::cout << "Canvas : key press\n";
    return false;
}

} // namespace maxwell
