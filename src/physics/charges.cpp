#include "charges.hpp"

namespace maxwell
{

void charges::clear()
{
  _positive_charges.clear();
  _negative_charges.clear();
}

bool charges::empty()
{
  return _positive_charges.empty() && _negative_charges.empty();
}

std::optional<charge&> charges::get_hint(const point& coord, charge::type type, double dist)
{
    const auto pred = [&coord, &dist](const charge& charge) {
        const auto delta = charge.get_coord() - coord;
        return delta.module() < dist;
    };

    const auto hint = [&pred](data_t& charges) -> std::optional<charge&>
    {
        auto it = std::find_if(charges.begin(), charges.end(), pred);
        if (it != charges.end()) {
            return std::ref(*it);
        }
        return {};
    };

    std::optional<charge&> res;
    switch (type) {
    case charge::type::negative: {
        res = hint(_negative_charges);
        break;
    }
    case charge::type::positive: {
        res = hint(_positive_charges);
        break;
    }
    case charge::type::any: {
        res = hint(_negative_charges);
        if (!res.has_value()) {
          res = hint(positive_charges);
        }
        break;
    }
    }
    return res;
}

const data_t& charges::get_positive_charges() const
{
  return _positive_charges;
}

const data_t& charges::get_negative_charges() const
{
  return _negative_charges;
}

} // namespace maxwell

