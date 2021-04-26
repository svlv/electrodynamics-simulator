#include "graphics/widgets/charge_props.hpp"
#include <gtkmm/arrow.h>
namespace maxwell
{

const std::string label_coord_x = "label-coord-x";
const std::string label_coord_y = "label-coord-y";
const std::string label_value = "label-value";
const std::string entry_coord_x = "entry-coord-x";
const std::string entry_coord_y = "entry-coord-y";
const std::string entry_value = "entry-value";
const std::string arrow_coord_x_left = "arrow-coord-x-left";
const std::string arrow_coord_x_right = "arrow-coord-x-right";
const std::string arrow_coord_y_up = "arrow-coord-y-up";
const std::string arrow_coord_y_down = "arrow-coord-y-down";
const std::string arrow_value_up = "arrow-value-up";
const std::string arrow_value_down = "arrow-value-down";
const std::string button_coord_x_left = "button-coord-x-left";
const std::string button_coord_x_right = "button-coord-x-right";
const std::string button_coord_y_up = "button-coord-y-up";
const std::string button_coord_y_down = "button-coord-y-down";
const std::string button_value_up = "button-value-up";
const std::string button_value_down = "button-value-down";
const std::string box_coord_x = "box-coord-x";
const std::string box_coord_y = "box-coord-y";
const std::string box_value = "box-value";

struct arrow_button {
    std::unique_ptr<Gtk::Arrow> arrow;
    std::unique_ptr<Gtk::Button> button;
};

struct hbox {
    std::unique_ptr<Gtk::Label> label;
    std::unique_ptr<Gtk::Entry> entry;
    std::unique_ptr<arrow_button> button1;
    std::unique_ptr<arrow_button> button2;
    std::unique_ptr<Gtk::Box> box;
};

arrow_button make_arrow_button(Gtk::ArrowType type)
{
    auto button = std::make_unique<Gtk::Button>();
    auto arrow =
        std::make_unique<Gtk::Arrow>(type, Gtk::ShadowType::SHADOW_ETCHED_OUT);
    button->add(*arrow);
    return {std::move(arrow), std::move(button)};
}

box make_box(Gtk::ArrowType arrow_type1, Gtk::ArrowType arrow_type2)
{
    auto label = std::make_unique<Gtk::Label>();
    auto entry = std::make_unique<Gtk::Entry>();
    entry->set_hexpand(true);
    auto button1 = make_arrow_button(arrow_type1);
    auto button2 = make_arrow_button(arrow_type2);
    auto box = std::make_unique<Gtk::Box>(
        Gtk::Orientation::ORIENTATION_HORIZONTAL, 10);
    box->add(*label);
    box->add(*entry);
    box->add(*button1);
    box->add(*button2);
    return {std::move(label), std::move(entry), std::move(button1),
            std::move(button2), std::move(box)};
}

switch (type) {
case Gtk::ArrowType::ARROW_UP:
    button->signal_clicked().connect(
        sigc::mem_fun(*this, &charge_props::_on_button_charge_up_click));
    break;
case Gtk::ArrowType::ARROW_DOWN:
    button->signal_clicked().connect(
        sigc::mem_fun(*this, &charge_props::_on_button_charge_down_click));
    break;
case Gtk::ArrowType::ARROW_RIGHT:
    button->signal_clicked().connect(
        sigc::mem_fun(*this, &charge_props::_on_button_charge_right_click));
    break;
case Gtk::ArrowType::ARROW_LEFT:
    button->signal_clicked().connect(
        sigc::mem_fun(*this, &charge_props::_on_button_charge_left_click));
    break;
}

charge_props::charge_props(Gtk::Window& parent, const charge_ptr& charge,
                           Gtk::DrawingArea& area)
    : Gtk::Dialog("", parent,
                  Gtk::DIALOG_MODAL | Gtk::DIALOG_DESTROY_WITH_PARENT |
                      Gtk::DIALOG_USE_HEADER_BAR),
      _charge(charge), _drawing_area(area)
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

void charge_props::create_arrow(Gtk::ArrowType type)
{
    const auto add_arrow = [&](Gtk::ArrowType type) {
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
