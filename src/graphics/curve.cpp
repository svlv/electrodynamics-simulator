#include "graphics/curve.hpp"

namespace maxwell
{

void curve::add_point(const point& coord)
{
  _points.push_back(coord);
}

void curve::draw(const Cairo::RefPtr<Cairo::Context>& ctx)
{
    if (_points.size() < 4U) {
      return;
    }
    ctx->move_to(_points[0U].x, _points[0U].y);
    for (size_t idx = 0; idx + 3 < _points.size(); idx+=3) {
      const auto& p1 = _points[idx + 1U];
      const auto& p2 = _points[idx + 2U];
      const auto& p3 = _points[idx + 3U];
      ctx->curve_to(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);
    }
    ctx->stroke();
}

} // namespace maxwell

