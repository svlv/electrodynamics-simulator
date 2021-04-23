#pragma once
#include "graphics/point.hpp"
#include <gdkmm/rgba.h>
#include <cairomm/context.h>
namespace maxwell
{
  class square
  {
    public:
      explicit square(const point& coord);
      square(const point& coord, const Gdk::RGBA& color);
      const point& get_coord() const;
      void set_color(const Gdk::RGBA& color);
      void draw(const Cairo::RefPtr<Cairo::Context>& ctx) const;
      static double size;
    private:
      point _coord;
      Gdk::RGBA _color;
  };
} // namespace maxwell
