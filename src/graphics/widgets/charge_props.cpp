#include "graphics/widgets/charge_props.hpp"
#include <gtkmm/arrow.h>
namespace maxwell
{

charge_props::charge_props(Gtk::Window& parent, const charge_ptr& charge,Gtk::DrawingArea& area )
    : Gtk::Dialog("", parent,
                  Gtk::DIALOG_MODAL | Gtk::DIALOG_DESTROY_WITH_PARENT |
                      Gtk::DIALOG_USE_HEADER_BAR),
      _charge(charge)
      , _drawing_area(area)
{
    set_size_request(200, 100);
    add_button("Cancel", Gtk::ResponseType::RESPONSE_CANCEL);
    add_button("Ok", Gtk::ResponseType::RESPONSE_OK);

    auto* content = get_content_area();
    add_box("Value: ", std::to_string(static_cast<int>(charge->get_value())));
    add_box("x: ", std::to_string(static_cast<int>(_charge->get_coord().x)));
    add_box("y: ", std::to_string(static_cast<int>(_charge->get_coord().y)));
    auto box =
        std::make_unique<Gtk::Box>(Gtk::Orientation::ORIENTATION_VERTICAL);
    box->add(*_widgets["box1"]);
    box->add(*_widgets["box2"]);
    box->add(*_widgets["box3"]);
    add_arrows();
    box->add(*_widgets["box-for-arrows"]);
    _frame.set_label("Charge props");
    //_frame.set_label_align(Gtk::ALIGN_END, Gtk::ALIGN_START);
    _frame.set_shadow_type(Gtk::SHADOW_ETCHED_OUT);
    _frame.add(*box);
    _frame.set_vexpand(true);
    _frame.set_hexpand(true);

    _widgets["box"] = std::move(box);
    content->add(_frame);
    show_all();
}

void charge_props::_on_button_charge_up_click()
{
    _charge->get_coord().y -= 10;
    _drawing_area.get().queue_draw();
}

void charge_props::_on_button_charge_down_click()
{
    _charge->get_coord().y += 10;
    _drawing_area.get().queue_draw();
}

void charge_props::_on_button_charge_left_click()
{
    _charge->get_coord().x -= 10;
    _drawing_area.get().queue_draw();
}

void charge_props::_on_button_charge_right_click()
{
    _charge->get_coord().x += 10;
    _drawing_area.get().queue_draw();
}

void charge_props::add_box(const Glib::ustring& label,
                           const Glib::ustring& entry)
{
    ++_idx;
    auto lbl = std::make_unique<Gtk::Label>(label);
    auto entr = std::make_unique<Gtk::Entry>();
    entr->set_text(entry);
    entr->set_hexpand(true);
    auto box = std::make_unique<Gtk::Box>(
        Gtk::Orientation::ORIENTATION_HORIZONTAL, 10);
    box->add(*lbl);
    box->add(*entr);
    _widgets["label" + std::to_string(_idx)] = std::move(lbl);
    _widgets["entry" + std::to_string(_idx)] = std::move(entr);
    _widgets["box" + std::to_string(_idx)] = std::move(box);
}

std::string get_key_for_arrow(Gtk::ArrowType type)
{
    return "arrow_" + std::to_string(type);
}

std::string get_key_for_button_arrow(Gtk::ArrowType type)
{
    return "button_arrow_" + std::to_string(type);
}

void charge_props::add_arrows()
{
    auto box =
        std::make_unique<Gtk::Box>(Gtk::Orientation::ORIENTATION_HORIZONTAL);
    const auto add_arrow = [&](Gtk::ArrowType type) {
        auto button = std::make_unique<Gtk::Button>();
        switch (type)
        {
          case Gtk::ArrowType::ARROW_UP:
              button->signal_clicked().connect(sigc::mem_fun(*this, &charge_props::_on_button_charge_up_click));
              break;
          case Gtk::ArrowType::ARROW_DOWN:
              button->signal_clicked().connect(sigc::mem_fun(*this, &charge_props::_on_button_charge_down_click));
              break;
          case Gtk::ArrowType::ARROW_RIGHT:
              button->signal_clicked().connect(sigc::mem_fun(*this, &charge_props::_on_button_charge_right_click));
              break;
          case Gtk::ArrowType::ARROW_LEFT:
              button->signal_clicked().connect(sigc::mem_fun(*this, &charge_props::_on_button_charge_left_click));
              break;
        }
        auto arrow = std::make_unique<Gtk::Arrow>(
            type, Gtk::ShadowType::SHADOW_ETCHED_OUT);
        button->add(*arrow);
        box->add(*button);
        _widgets[get_key_for_arrow(type)] = std::move(arrow);
        _widgets[get_key_for_button_arrow(type)] = std::move(button);
    };
    add_arrow(Gtk::ArrowType::ARROW_LEFT);
    add_arrow(Gtk::ArrowType::ARROW_DOWN);
    add_arrow(Gtk::ArrowType::ARROW_UP);
    add_arrow(Gtk::ArrowType::ARROW_RIGHT);
    _widgets["box-for-arrows"] = std::move(box);
}

void charge_props::on_response(int response_id)
{
    if (response_id == Gtk::ResponseType::RESPONSE_OK) {
        const auto* entry_x =
            dynamic_cast<Gtk::Entry*>(_widgets["entry2"].get());
        if (entry_x != nullptr) {
            auto x = std::stoi(entry_x->get_text().c_str());
            _charge->get_coord().x = x;
        }
        const auto* entry_y =
            dynamic_cast<Gtk::Entry*>(_widgets["entry3"].get());
        if (entry_y != nullptr) {
            auto y = std::stoi(entry_y->get_text().c_str());
            _charge->get_coord().y = y;
        }
        const auto* entry_val =
            dynamic_cast<Gtk::Entry*>(_widgets["entry1"].get());
        if (entry_val != nullptr) {
            auto val = std::stoi(entry_val->get_text().c_str());
            _charge->set_value(val);
        }
    }
    Gtk::Dialog::on_response(response_id);
}

} // namespace maxwell
