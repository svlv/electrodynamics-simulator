#pragma once
#include "graphics/base_line.hpp"
namespace maxwell
{
class curve : public base_line
{
  public:
    using base_line::base_line;
    void fill() override;
    ~curve() override = default;
    void draw(const Cairo::RefPtr<Cairo::Context>& ctx) const override;
};
} // namespace maxwell
