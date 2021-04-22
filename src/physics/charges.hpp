#pragma once

#include "charge.hpp"
#include <memory>
#include <optional>
#include <vector>

namespace maxwell
{

class charges
{
  public:
    using data_t = std::vector<charge_ptr>;
    charges();
    template <typename... Ts> void emplace_back(charge::type type, Ts&&... args)
    {
        switch (type) {
        case charge::type::positive:
            _positive_charges.emplace_back(
                std::make_shared<charge>(std::forward<Ts>(args)...));
            break;
        case charge::type::negative:
            _negative_charges.emplace_back(
                std::make_shared<charge>(std::forward<Ts>(args)...));
            break;
        }
    }
    void clear();
    bool empty();

    charge_ptr get_hint(const point& coord, charge::type type, double dist);

    const data_t& get_positive_charges() const;
    const data_t& get_negative_charges() const;

    //data_t& get_positive_charges();
    //data_t& get_negative_charges();

  private:
    data_t _positive_charges;
    data_t _negative_charges;
};

} // namespace maxwell
