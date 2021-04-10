#pragma once

#include "charge.hpp"
#include <vector>
#include <optional>

namespace maxwell
{

class charges
{
  public:
    using data_t = std::vector<charge>;

    template <typename... Ts> void emplace_back(charge::type type, Ts&&... args)
    {
        switch (type) {
        case charge::type::positive:
            _positive_charges.emplace_back(std::forward<Ts>(args)...);
            break;
        case charge::type::negative:
            _negative_charges.emplace_back(std::forward<Ts>(args)...);
            break;
        }
    }
    void clear();
    bool empty();

    charge* get_hint(const point& coord, charge::type type, double dist);

    const data_t& get_positive_charges() const;
    const data_t& get_negative_charges() const;

  private:
    data_t _positive_charges;
    data_t _negative_charges;
};

} // namespace maxwell

