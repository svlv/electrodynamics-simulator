#include "Utils.hxx"

#include <vector>

namespace maxwell
{

void draw_arrow(const Cairo::RefPtr<Cairo::Context>& cr,
                Coordinates coord, const ArrowSize& size)
{
    const std::vector<Coordinates> points = {
        {-1.0*size.a, (size.b+size.d)/2.0},
        {size.a, 0.0},
        {0.0, -1.0*size.b},
        {size.c, 0.0},
        {-1*(size.a/2.0+size.c), -1.0*size.d},
        {-1*(size.a/2.0+size.c), size.d},
        {size.c, 0.0},
        {0.0, size.b}
    };

    coord += points[0];
    cr->move_to(coord.x, coord.y);
    for (size_t idx = 1; idx < points.size(); ++idx)
    {
        coord += points[idx];
        cr->line_to(coord.x, coord. y);
    }

    cr->stroke();
}

} // namespace maxwell
