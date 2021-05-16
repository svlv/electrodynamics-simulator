#include "graphics/curve.hpp"
#include "graphics/context_guard.hpp"

#include <gdkmm/general.h>

namespace elfield
{

void curve::fill()
{
    const auto sz = _points.size();
    const auto mod = sz % 3U;
    if (mod == 0U) {
        _points.push_back(_points.back());
    } else if (mod == 2U) {
        _points.push_back(_points.back());
        _points.push_back(_points.back());
    }
}

void curve::draw(const Cairo::RefPtr<Cairo::Context>& ctx) const
{
    const auto sz = _points.size();
    if (sz < 4U) {
        return;
    }
    const auto guard = context_guard(ctx);
    if (_color.has_value()) {
        Gdk::Cairo::set_source_rgba(ctx, _color.value());
    }
    ctx->move_to(_points[0U].x, _points[0U].y);
    for (size_t idx = 0U; idx + 3U < sz; idx += 3U) {
        const auto& p1 = _points[idx + 1U];
        const auto& p2 = _points[idx + 2U];
        const auto& p3 = _points[idx + 3U];
        ctx->curve_to(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);
    }
    ctx->stroke();
    draw_chevrons(ctx);
}

} // namespace elfield
