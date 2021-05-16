#include "graphics/line.hpp"
#include "graphics/context_guard.hpp"
#include <gdkmm/general.h>

namespace elfield
{

void line::draw(const Cairo::RefPtr<Cairo::Context>& ctx) const
{
    const auto sz = _points.size();
    if (sz < 2U) {
        return;
    }
    const auto guard = context_guard(ctx);
    if (_color.has_value()) {
        Gdk::Cairo::set_source_rgba(ctx, _color.value());
    }
    ctx->move_to(_points[0U].x, _points[0U].y);
    for (size_t idx = 1U; idx < sz; ++idx) {
        ctx->line_to(_points[idx].x, _points[idx].y);
    }
    ctx->stroke();
    draw_chevrons(ctx);
}

} // namespace elfield
