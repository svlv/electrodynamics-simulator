#pragma once

#include "charge.hpp"
#include <memory>
#include <optional>
#include <vector>

#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index_container.hpp>

namespace maxwell
{

struct by_value;
struct by_coord;

struct value_extractor {
    typedef double result_type;
    result_type operator()(const charge_ptr& chrg) const
    {
        return chrg->get_value();
    }
};

struct coord_extractor {
    typedef point result_type;
    const result_type& operator()(const charge_ptr& chrg) const
    {
        return chrg->get_coord();
    }
};

using boost::multi_index::indexed_by;
using boost::multi_index::member;
using boost::multi_index::ordered_non_unique;
using boost::multi_index::ordered_unique;
using boost::multi_index::tag;

typedef boost::multi_index_container<
    charge_ptr, indexed_by<ordered_non_unique<tag<by_value>, value_extractor>,
                           ordered_non_unique<tag<by_coord>, coord_extractor>>>
    charges_container;

class charges
{
  public:
    using data_t = std::vector<charge_ptr>;
    charges();
    template <typename... Ts> void emplace_back(charge::type type, Ts&&... args)
    {
        auto chrg = std::make_shared<charge>(std::forward<Ts>(args)...);
        switch (type) {
        case charge::type::positive:
            _positive_charges.emplace_back(chrg);
            break;
        case charge::type::negative:
            _negative_charges.emplace_back(chrg);
            break;
        }
        _charges.emplace(chrg);
    }
    void clear();
    bool empty();
    void erase(charge_ptr chrg);

    charge_ptr get_hint(const point& coord, charge::type type, double dist);

    const data_t& get_positive_charges() const;
    const data_t& get_negative_charges() const;

    const charges_container& get_charges() { return _charges; }

    // data_t& get_positive_charges();
    // data_t& get_negative_charges();

  private:
    data_t _positive_charges;
    data_t _negative_charges;
    charges_container _charges;
};

} // namespace maxwell
