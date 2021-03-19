#pragma once

#include "utils.hpp"
#include <vector>
#include <optional>

namespace maxwell
{

struct Charge
{
    Charge(const Coordinates& coordinates_, int value_)
        : coordinates(coordinates_)
        , value(value_) {}
    Charge(double x_, double y_, int value_)
        : coordinates(x_, y_)
        , value(value_) {}

    Coordinates coordinates;
    int value;
};

enum class ChargeType
{
    Negative = 0,
    Positive = 1
};

class Charges
{
public:
    Charges() = default;

    using Data = std::vector<Charge>;
    using Iter = Data::iterator;
    using ConstIter = Data::const_iterator;

    template <typename... Ts>
    void emplaceBackPositiveCharge(Ts&&... args)
    {
        _positiveCharges.emplace_back(std::forward<Ts>(args)...);
    }

    template <typename... Ts>
    void emplaceBackNegativeCharge(Ts&&... args)
    {
        _negativeCharges.emplace_back(std::forward<Ts>(args)...);
    }

    void clear();
    bool empty();

    double getEx(const Coordinates& coordinates) const;
    double getEy(const Coordinates& coordinates) const;
    double getE (const Coordinates& coordinates) const;

    double getCos(const Coordinates& coordinates) const;
    double getSin(const Coordinates& coordinates) const;

    std::optional<Coordinates> isComeToNegative(const Coordinates& coordinates) const;
    std::optional<Coordinates> isComeToPositive(const Coordinates& coordinates) const;

    const Data& getPositiveCharges() const;
    const Data& getNegativeCharges() const;

private:
    std::optional<Coordinates> isNear(const Coordinates& coordinates, ChargeType type) const;

    Data _positiveCharges;
    Data _negativeCharges;
};

} // namespace maxwell
