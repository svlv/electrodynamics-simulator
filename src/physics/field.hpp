#pragma once

#include <optional>
#include <vector>

#include "graphics/point.hpp"
#include "physics/charge.hpp"

namespace maxwell
{

class field
{
  public:
    field() = default;

    using Data = std::vector<charge>;
    using Iter = Data::iterator;
    using ConstIter = Data::const_iterator;

    template <typename... Ts> void emplaceBackPositiveCharge(Ts&&... args)
    {
        _positiveCharges.emplace_back(std::forward<Ts>(args)...);
    }

    template <typename... Ts> void emplaceBackNegativeCharge(Ts&&... args)
    {
        _negativeCharges.emplace_back(std::forward<Ts>(args)...);
    }

    void clear();
    bool empty();

    double getEx(const point& coordinates) const;
    double getEy(const point& coordinates) const;
    double getE(const point& coordinates) const;

    double getCos(const point& coordinates) const;
    double getSin(const point& coordinates) const;

    std::optional<point> isComeToNegative(const point& coordinates) const;
    std::optional<point> isComeToPositive(const point& coordinates) const;

    const Data& getPositiveCharges() const;
    const Data& getNegativeCharges() const;

  private:
    std::optional<point> isNear(const point& coordinates,
                                charge::type type) const;

    Data _positiveCharges;
    Data _negativeCharges;
};

} // namespace maxwell