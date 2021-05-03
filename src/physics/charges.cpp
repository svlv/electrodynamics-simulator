#include "charges.hpp"
#include <algorithm>

namespace elfield
{

charges::charges() = default;

void charges::clear()
{
    _positive_charges.clear();
    _negative_charges.clear();
}

bool charges::empty()
{
    return _positive_charges.empty() && _negative_charges.empty();
}

charge_ptr charges::get_hint(const point& coord, charge::type type, double dist)
{
    const auto pred = [&coord, &dist](const charge_ptr& charge) {
        if (charge == nullptr) {
            return false;
        }
        const auto delta = charge->get_coord() - coord;
        return delta.module() < dist;
    };

    const auto hint = [&pred](data_t& charges) -> charge_ptr {
        auto it = std::find_if(charges.begin(), charges.end(), pred);
        if (it != charges.end()) {
            return *it;
        }
        return charge_ptr(nullptr);
    };

    charge_ptr res;
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
        if (!res) {
            res = hint(_positive_charges);
        }
        break;
    }
    }

    return res;
}

const charges::data_t& charges::get_positive_charges() const
{
    return _positive_charges;
}

const charges::data_t& charges::get_negative_charges() const
{
    return _negative_charges;
}

void charges::erase(charge_ptr chrg)
{
    const auto erase_chrg = [&](data_t& charges) {
        const auto it = std::find(charges.cbegin(), charges.cend(), chrg);
        if (it != charges.cend()) {
            charges.erase(it);
        }
    };
    if (chrg->get_value() > 0.0) {
        erase_chrg(_positive_charges);
    } else {
        erase_chrg(_negative_charges);
    }
}

void charges::validate(charge_ptr chrg)
{
  const auto validate_chrg = [&chrg](data_t& src, data_t& dest) {
    auto it = std::find(src.cbegin(), src.cend(), chrg);
    if (it != src.cend()) {
      return;
    }
    it = std::find(dest.cbegin(), dest.cend(), chrg);
    if (it != dest.cend()) {
        dest.erase(it);
        src.emplace_back(std::move(chrg));
    }
  };
  if (chrg->get_value() > 0.0) {
    validate_chrg(_positive_charges, _negative_charges);
  }
  else if (chrg->get_value() < 0.0) {
    validate_chrg(_negative_charges, _positive_charges);
  }
}

} // namespace elfield
