#pragma once

#include "graphics/chevron.hpp"
#include "graphics/point.hpp"
#include <cairomm/context.h>
#include <memory>
#include <vector>

namespace elfield
{
class base_line
{
  public:
    enum class type : uint8_t { line = 0, curve = 1 };
    explicit base_line(const point& begin);
    virtual ~base_line() = default;
    base_line() = default;
    void add_point(const point& coord);
    void add_chevron(const chevron& chvrn);
    virtual void fill();
    virtual void draw(const Cairo::RefPtr<Cairo::Context>& ctx) const = 0;

  protected:
    void draw_chevrons(const Cairo::RefPtr<Cairo::Context>& ctx) const;
    std::vector<point> _points;
    std::vector<chevron> _chevrons;
};
using base_line_uptr = std::unique_ptr<base_line>;
} // namespace elfield
