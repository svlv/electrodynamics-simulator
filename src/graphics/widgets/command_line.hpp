#pragma once

#include <gtkmm/entry.h>

namespace elfield
{

class canvas;

class history
{
  public:
    history();
    void add(const std::string& line);
    std::string prev();
    std::string next();
    void reset();

  private:
    std::vector<std::string> _base;
    size_t _pos;
};

class command_line : public Gtk::Entry
{
  public:
    explicit command_line(canvas& cnvs);

  protected:
    bool on_key_press_event(GdkEventKey* event) override;
    void on_delete_text(int start_pos, int end_pos) override;

  private:
    void process();

  private:
    canvas& _canvas;
    history _hist;
};

} // namespace elfield
