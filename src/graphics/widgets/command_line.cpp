#include "graphics/widgets/command_line.hpp"
#include "graphics/canvas.hpp"
#include <gdkmm/rgba.h>
#include <iostream>

namespace elfield
{

void history::add(const std::string& line)
{
    _base.push_back(line);
    _pos = _base.size();
}

std::string history::prev()
{
    if (_base.empty()) {
        return std::string();
    }
    if (_pos != 0U ) {
      --_pos;
      return _base[_pos];
    } else if (_base.size() == 1U) {
      return _base[_pos];
    }
    return std::string();
}

std::string history::next()
{
    if (_base.empty()) {
        return std::string();
    }
    if (_pos == _base.size() - 1) {
      ++_pos;
      return std::string();
    } else if (_pos == _base.size()) {
      return std::string();
    }
    ++_pos;
    return _base[_pos];
}

void history::reset() {
    _pos = _base.size();
}

command_line::command_line(canvas& cnvs) : _canvas(cnvs)
{
    add_events(Gdk::KEY_PRESS_MASK);
}

bool command_line::on_key_press_event(GdkEventKey* event)
{
    if (event->keyval == GDK_KEY_Return) {
        if (get_text() != ":") {
            _hist.add(get_text());
        }
        process();
        set_text(":");
        set_position(1);
        return true;
    } else if (event->keyval == GDK_KEY_Up) {
        const auto& line = _hist.prev();
        if (!line.empty()) {
            set_text(line);
            set_position(line.size());
        }
        return true;
    } else if (event->keyval == GDK_KEY_Down) {
        const auto& line = _hist.next();
        if (!line.empty()) {
            set_text(line);
            set_position(line.size());
        } else {
          set_text(":");
          set_position(1);
        }
        return true;
    }
    return Gtk::Entry::on_key_press_event(event);
}

void command_line::on_delete_text(int start_pos, int end_pos)
{
    if (start_pos != 0 || end_pos == -1) {
        Gtk::Entry::on_delete_text(start_pos, end_pos);
    } else if (start_pos == 0 && get_text().size() == end_pos) {
        Gtk::Entry::on_delete_text(start_pos, end_pos);
        hide();
        _hist.reset();
        _canvas.grab_focus();
    }
}

void command_line::process()
{
    std::istringstream ss(get_text());
    std::string func_name;
    ss >> func_name;
    if (func_name == ":set_background_color") {
        std::string color;
        ss >> color;
        Gdk::RGBA rgba(color);
        _canvas.set_background_color(rgba);
    }
}

} // namespace elfield
