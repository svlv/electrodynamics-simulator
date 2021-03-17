#include "Utils.hxx"

#include <vector>
#include <math.h>

namespace maxwell
{

void draw_arrow(const Cairo::RefPtr<Cairo::Context>& cr,
                Coordinates coord, float sin_, float cos_, const ArrowSize& size)
{
    std::vector<Coordinates> points = {
        {-1.0*size.a/2, (size.b+size.d)/2.0},
        {size.a, 0.0},
        {0.0, -1.0*size.b},
        {size.c, 0.0},
        {-1*(size.a/2.0+size.c), -1.0*size.d},
        {-1*(size.a/2.0+size.c), size.d},
        {size.c, 0.0},
        {0.0, size.b}
    };

    for (auto& point : points)
    {
        rotate(point, -1, 0);
    }

    for (auto& point : points)
    {
        rotate(point, sin_, cos_);
    }

    coord += points[0];
    cr->move_to(coord.x, coord.y);
    for (size_t idx = 1; idx < points.size(); ++idx)
    {
        coord += points[idx];
        cr->line_to(coord.x, coord. y);
    }

    cr->stroke();
}

void rotate(Coordinates& coord, float sin_, float cos_)
{
    const auto x_ =  cos_ * coord.x - sin_ * coord.y;
    const auto y_ =  sin_ * coord.x + cos_ * coord.y;
    coord.x = x_;
    coord.y = y_;
}


} // namespace maxwell
