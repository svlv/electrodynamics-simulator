#pragma once
#include "graphics/base_line.hpp"
namespace elfield
{
class line : public base_line
{
  public:
    using base_line::base_line;
    ~line() override = default;
    void draw(const Cairo::RefPtr<Cairo::Context>& ctx) const override;
};
} // namespace elfield
