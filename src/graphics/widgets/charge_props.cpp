#include "graphics/widgets/charge_props.hpp"
#include <gtkmm/arrow.h>
namespace maxwell
{

struct arrow_button {
    explicit arrow_button(Gtk::ArrowType arrow_type)
        : button(std::make_unique<Gtk::Button>()),
          arrow(std::make_unique<Gtk::Arrow>(
              arrow_type, Gtk::ShadowType::SHADOW_ETCHED_OUT))
    {
        button->add(*arrow);
    }
    void pack(const std::string& postfix, charge_props::widgets_t& widgets)
    {
        widgets["button-" + postfix] = std::move(button);
        widgets["arrow-" + postfix] = std::move(arrow);
    }
    std::unique_ptr<Gtk::Arrow> arrow;
    std::unique_ptr<Gtk::Button> button;
};

struct hbox {
    explicit hbox(Gtk::ArrowType arrow_type1, Gtk::ArrowType arrow_type2)
        : label(std::make_unique<Gtk::Label>()),
          entry(std::make_unique<Gtk::Entry>()),
          button1(std::make_unique<arrow_button>(arrow_type1)),
          button2(std::make_unique<arrow_button>(arrow_type2)),
          box(std::make_unique<Gtk::Box>(
              Gtk::Orientation::ORIENTATION_HORIZONTAL, 10))
    {
        box->add(*label);
        box->add(*entry);
        box->add(*(button1->button));
        box->add(*(button2->button));
    }

    void pack(const std::string& postfix, charge_props::widgets_t& widgets)
    {
        widgets["label-" + postfix] = std::move(label);
        widgets["entry-" + postfix] = std::move(entry);
        button1->pack("1" + postfix, widgets);
        button2->pack("2" + postfix, widgets);
        widgets["box-" + postfix] = std::move(box);
    }

    std::unique_ptr<Gtk::Label> label;
    std::unique_ptr<Gtk::Entry> entry;
    std::unique_ptr<arrow_button> button1;
    std::unique_ptr<arrow_button> button2;
    std::unique_ptr<Gtk::Box> box;
};

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

    auto box_x = hbox(Gtk::ArrowType::ARROW_LEFT, Gtk::ArrowType::ARROW_RIGHT);
    box_x.label->set_text("x: ");
    box_x.entry->set_text(
        std::to_string(static_cast<int>(_charge->get_coord().x)));
    box_x.button1->button->signal_clicked().connect(
        sigc::mem_fun(*this, &charge_props::_on_button_charge_left_click));
    box_x.button2->button->signal_clicked().connect(
        sigc::mem_fun(*this, &charge_props::_on_button_charge_right_click));

    auto box_y = hbox(Gtk::ArrowType::ARROW_DOWN, Gtk::ArrowType::ARROW_UP);
    box_y.label->set_text("y: ");
    box_y.entry->set_text(
        std::to_string(static_cast<int>(_charge->get_coord().y)));
    box_y.button1->button->signal_clicked().connect(
        sigc::mem_fun(*this, &charge_props::_on_button_charge_down_click));
    box_y.button2->button->signal_clicked().connect(
        sigc::mem_fun(*this, &charge_props::_on_button_charge_up_click));

    auto box_value = hbox(Gtk::ArrowType::ARROW_DOWN, Gtk::ArrowType::ARROW_UP);
    box_value.label->set_text("Value: ");
    box_value.entry->set_text(
        std::to_string(static_cast<int>(charge->get_value())));

    auto main_box =
        std::make_unique<Gtk::Box>(Gtk::Orientation::ORIENTATION_VERTICAL);
    main_box->add(*box_x.box);
    main_box->add(*box_y.box);
    main_box->add(*box_value.box);

    box_x.pack("x", _widgets);
    box_y.pack("y", _widgets);
    box_value.pack("value", _widgets);

    _frame.set_label("Charge props");
    _frame.set_shadow_type(Gtk::SHADOW_ETCHED_OUT);
    _frame.add(*main_box);
    _frame.set_vexpand(true);
    _frame.set_hexpand(true);
    _widgets["main-box"] = std::move(main_box);
    auto* content = get_content_area();
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

void charge_props::on_response(int response_id)
{
    if (response_id == Gtk::ResponseType::RESPONSE_OK) {
        const auto* entry_x =
            dynamic_cast<Gtk::Entry*>(_widgets["entry-x"].get());
        if (entry_x != nullptr) {
            auto x = std::stoi(entry_x->get_text().c_str());
            _charge->get_coord().x = x;
        }
        const auto* entry_y =
            dynamic_cast<Gtk::Entry*>(_widgets["entry-y"].get());
        if (entry_y != nullptr) {
            auto y = std::stoi(entry_y->get_text().c_str());
            _charge->get_coord().y = y;
        }
        const auto* entry_val =
            dynamic_cast<Gtk::Entry*>(_widgets["entry-value"].get());
        if (entry_val != nullptr) {
            auto val = std::stoi(entry_val->get_text().c_str());
            _charge->set_value(val);
        }
    }
    Gtk::Dialog::on_response(response_id);
}

} // namespace maxwell
