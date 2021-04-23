#pragma once

#include "graphics/point.hpp"
#include <cairomm/context.h>
#include <memory>
#include <vector>

namespace maxwell
{
class base_line
{
  public:
    enum class type : uint8_t { line = 0, curve = 1 };
    explicit base_line(const point& begin);
    virtual ~base_line() = default;
    base_line() = default;
    void add_point(const point& coord);
    virtual void fill();
    virtual void draw(const Cairo::RefPtr<Cairo::Context>& ctx) const = 0;

  protected:
    std::vector<point> _points;
};
using base_line_uptr = std::unique_ptr<base_line>;
} // namespace maxwell
