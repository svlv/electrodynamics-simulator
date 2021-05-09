#pragma once

#include "graphics/primitive.hpp"

namespace elfield
{

class chevron : public primitive
{
  public:
    struct params {
        double d;
        angle_t angle;
    };
    chevron() = default;
    chevron(const params& size, const point& coord, angle_t angle);
    void draw(const Cairo::RefPtr<Cairo::Context>& context) const;

  private:
    std::vector<point> _init(const params& prms) const;
};

} // namespace elfield
