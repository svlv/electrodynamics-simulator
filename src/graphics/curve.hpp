#include "graphics/point.hpp"
#include <vector>
#include <cairomm/context.h>
namespace maxwell
{
class curve
{
public:
  curve() = default;
  void add_point(const point& coord);
  void draw(const Cairo::RefPtr<Cairo::Context>& ctx);
private:
  std::vector<point> _points;
};
} // namespace maxwell
