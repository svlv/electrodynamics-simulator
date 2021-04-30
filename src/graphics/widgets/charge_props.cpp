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
        // button->set_size_request(1);
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
              Gtk::Orientation::ORIENTATION_HORIZONTAL, 0))
    {
        label->set_size_request(40);
        label->set_xalign(1.0);
        // entry->set_size_request(50);
        entry->set_width_chars(8);
        entry->set_max_length(8);
        // entry->set_alignment(0.0);
        // entry->set_default_size(40);
        // label->set_justify(Gtk::Justification::JUSTIFY_LEFT);
        // label->set_valign(Gtk::ALIGN_START);
        // box->set_size_request(50);
        box->pack_start(*label, Gtk::PackOptions::PACK_SHRINK);
        box->pack_start(*entry, Gtk::PackOptions::PACK_SHRINK);
        box->pack_start(*(button1->button), Gtk::PackOptions::PACK_SHRINK);
        box->pack_start(*(button2->button), Gtk::PackOptions::PACK_SHRINK);
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
    std::unique_ptr<Gtk::Frame> frame;
};
charge_props::charge_props(Gtk::Window& parent, const charge_ptr& chrg,
                           Gtk::DrawingArea& area)
    : Gtk::Dialog("", parent,
                  Gtk::DIALOG_MODAL | Gtk::DIALOG_DESTROY_WITH_PARENT |
                      Gtk::DIALOG_USE_HEADER_BAR),
      _charge(chrg), _original(std::make_shared<charge>(*chrg)),
      _drawing_area(area)
{
    // set_size_request(150, 60);
    set_title("Charge");
    // set_default_size(150, 200);
    set_resizable(false);
    add_button("Cancel", Gtk::ResponseType::RESPONSE_CANCEL);
    add_button("Ok", Gtk::ResponseType::RESPONSE_OK);

    auto box_x = hbox(Gtk::ArrowType::ARROW_LEFT, Gtk::ArrowType::ARROW_RIGHT);
    box_x.label->set_text("x: ");
    box_x.entry->set_text(std::to_string((_charge->get_coord().x)));
    box_x.button1->button->signal_clicked().connect(
        sigc::mem_fun(*this, &charge_props::_on_button_charge_left_click));
    box_x.button2->button->signal_clicked().connect(
        sigc::mem_fun(*this, &charge_props::_on_button_charge_right_click));

    auto box_y = hbox(Gtk::ArrowType::ARROW_DOWN, Gtk::ArrowType::ARROW_UP);
    box_y.label->set_text("y: ");
    box_y.entry->set_text(std::to_string(_charge->get_coord().y));
    box_y.button1->button->signal_clicked().connect(
        sigc::mem_fun(*this, &charge_props::_on_button_charge_down_click));
    box_y.button2->button->signal_clicked().connect(
        sigc::mem_fun(*this, &charge_props::_on_button_charge_up_click));

    auto box_value = hbox(Gtk::ArrowType::ARROW_DOWN, Gtk::ArrowType::ARROW_UP);
    box_value.label->set_text("Value: ");
    box_value.entry->set_text(std::to_string(_charge->get_value()));
    box_value.button1->button->signal_clicked().connect(
        sigc::mem_fun(*this, &charge_props::_on_button_value_down_click));
    box_value.button2->button->signal_clicked().connect(
        sigc::mem_fun(*this, &charge_props::_on_button_value_up_click));

    auto main_box =
        std::make_unique<Gtk::Box>(Gtk::Orientation::ORIENTATION_VERTICAL);
    // main_box->set_size_request(50);
    main_box->pack_start(*box_x.box, Gtk::PackOptions::PACK_SHRINK);
    main_box->pack_start(*box_y.box, Gtk::PackOptions::PACK_SHRINK);
    main_box->pack_start(*box_value.box, Gtk::PackOptions::PACK_SHRINK);

    box_x.pack("x", _widgets);
    box_y.pack("y", _widgets);
    box_value.pack("value", _widgets);

    _frame.set_label("Properties");
    _frame.set_shadow_type(Gtk::SHADOW_ETCHED_OUT);
    _frame.add(*main_box);
    _frame.set_border_width(5);
    //_frame.set_size_request(120);
    _frame.set_vexpand(false);
    _frame.set_hexpand(false);
    _widgets["main-box"] = std::move(main_box);
    auto* content = get_content_area();
    auto* header = get_header_bar();
    header->set_size_request(100);
    header->set_has_subtitle(false);
    content->pack_start(_frame, Gtk::PackOptions::PACK_SHRINK);
    show_all();
}

void charge_props::_on_button_charge_up_click()
{
    _charge->get_coord().y -= 10;
    _update_entry_y();
}

void charge_props::_on_button_charge_down_click()
{
    _charge->get_coord().y += 10;
    _update_entry_y();
}

void charge_props::_on_button_charge_left_click()
{
    _charge->get_coord().x -= 10;
    _update_entry_x();
}

void charge_props::_on_button_charge_right_click()
{
    _charge->get_coord().x += 10;
    _update_entry_x();
}

void charge_props::_update_entry_x()
{
    auto* entry = dynamic_cast<Gtk::Entry*>(_widgets["entry-x"].get());
    if (entry != nullptr) {
        entry->set_text(std::to_string(_charge->get_coord().x));
    }
    _drawing_area.get().queue_draw();
}

void charge_props::_update_entry_y()
{
    auto* entry = dynamic_cast<Gtk::Entry*>(_widgets["entry-y"].get());
    if (entry != nullptr) {
        entry->set_text(std::to_string(_charge->get_coord().y));
    }
    _drawing_area.get().queue_draw();
}

void charge_props::_update_entry_value()
{
    auto* entry = dynamic_cast<Gtk::Entry*>(_widgets["entry-value"].get());
    if (entry != nullptr) {
        entry->set_text(std::to_string(_charge->get_value()));
    }
    _drawing_area.get().queue_draw();
}

void charge_props::on_response(int response_id)
{
    if (response_id == Gtk::ResponseType::RESPONSE_CANCEL) {
        *_charge = *_original;
    }
    Gtk::Dialog::on_response(response_id);
}

void charge_props::_on_button_value_up_click()
{
    _charge->set_value(_charge->get_value() + 1.0);
    _update_entry_value();
}

void charge_props::_on_button_value_down_click()
{
    _charge->set_value(_charge->get_value() - 1.0);
    _update_entry_value();
}

} // namespace maxwell
