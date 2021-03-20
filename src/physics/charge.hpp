#pragma once

#include "utils.hpp"
#include <optional>
#include <vector>

namespace maxwell
{

struct Charge {
    Charge(const point& coordinates_, int value_)
        : coordinates(coordinates_), value(value_)
    {
    }
    Charge(double x_, double y_, int value_)
        : coordinates(x_, y_), value(value_)
    {
    }

    point coordinates;
    int value;
};

enum class ChargeType { Negative = 0, Positive = 1 };

class Charges
{
  public:
    Charges() = default;

    using Data = std::vector<Charge>;
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
                                ChargeType type) const;

    Data _positiveCharges;
    Data _negativeCharges;
};

} // namespace maxwell
