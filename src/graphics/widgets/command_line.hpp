#pragma once

#include <gtkmm/entry.h>

namespace elfield
{

class canvas;

class history
{
  public:
    void add(const std::string& line)
    {
        _base.push_back(line);
        _pos = _base.size() - 1U;
    }
    std::string prev()
    {
        if (_base.empty()) {
            return std::string();
        }
        const auto& ret = _base[_pos];
        if (_pos == 0) {
            _pos = _base.size() - 1U;
        } else {
            --_pos;
        }
        return ret;
    }

  private:
    std::vector<std::string> _base;
    size_t _pos = 0;
};

class command_line : public Gtk::Entry
{
  public:
    explicit command_line(canvas& cnvs);

  private:
    bool on_key_press_event(GdkEventKey* event) override;
    void on_delete_text(int start_pos, int end_pos) override;

  private:
    canvas& _canvas;
    history _hist;
};

} // namespace elfield
