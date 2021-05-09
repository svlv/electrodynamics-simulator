#pragma once

#include "graphics/primitive.hpp"

namespace elfield
{

class arrow : public primitive
{
  public:
    struct params {
        double a;
        double b;
        double c;
        double d;
    };

    arrow(const params& size, const point& coord, angle_t angle);
    arrow() = default;
    ~arrow() override = default;

    void draw(const Cairo::RefPtr<Cairo::Context>& context, bool fill) const;
    bool is_hint(const point& coord);

    void select(bool flag);
    bool is_selected() const;

    const point& get_coord() const;

  private:
    std::vector<point> _init(const params& prms) const;

    bool _is_selected;
};

} // namespace elfield
