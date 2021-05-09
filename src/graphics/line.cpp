#include "graphics/line.hpp"

namespace elfield
{

void line::draw(const Cairo::RefPtr<Cairo::Context>& ctx) const
{
    const auto sz = _points.size();
    if (sz < 2U) {
        return;
    }
    ctx->move_to(_points[0U].x, _points[0U].y);
    for (size_t idx = 1U; idx < sz; ++idx) {
        ctx->line_to(_points[idx].x, _points[idx].y);
    }
    ctx->stroke();
    draw_chevrons(ctx);
}

} // namespace elfield
