#pragma once

#include "physics/charge.hpp"
#include <gtkmm/dialog.h>
#include <gtkmm/entry.h>
#include <gtkmm/frame.h>
#include <gtkmm/label.h>
#include <map>
#include <string>

namespace maxwell
{
class charge_props : public Gtk::Dialog
{
  public:
    explicit charge_props(Gtk::Window& parent, const charge_ptr& charge);
    ~charge_props() = default;

  protected:
    void on_response(int response_id) override;

  private:
    void add_box(const Glib::ustring& label, const Glib::ustring& entry);

    charge_ptr _charge;
    std::map<std::string, std::unique_ptr<Gtk::Widget>> _widgets;
    Gtk::Frame _frame;
    size_t _idx = 0;
};
} // namespace maxwell
