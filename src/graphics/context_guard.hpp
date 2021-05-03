#pragma once

#include <cairomm/context.h>

namespace elfield
{
class context_guard
{
  public:
    explicit context_guard(const Cairo::RefPtr<Cairo::Context>& cr) : _cr(cr)
    {
        _cr->save();
    }
    ~context_guard() { _cr->restore(); }

  private:
    const Cairo::RefPtr<Cairo::Context>& _cr;
};
} // namespace elfield
