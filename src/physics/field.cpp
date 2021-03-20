#include "physics/field.hpp"

#include <algorithm>
#include <cmath>
#include <numeric>

namespace maxwell
{

void field::clear()
{
    _positiveCharges.clear();
    _negativeCharges.clear();
}

bool field::empty()
{
    return _positiveCharges.empty() && _negativeCharges.empty();
}

double field::getEx(const point& coord) const
{
    const auto sumEx = [&coord](double Ex, const charge& charge) -> double {
        const auto& charge_coord = charge.get_coord();
        double dx = coord.x - charge_coord.x;
        double dy = coord.y - charge_coord.y;
        return Ex +
               charge.get_value() * dx / pow(pow(dx, 2.0) + pow(dy, 2.0), 1.5);
    };

    return std::accumulate(_positiveCharges.cbegin(), _positiveCharges.cend(),
                           0.0, sumEx) +
           std::accumulate(_negativeCharges.cbegin(), _negativeCharges.cend(),
                           0.0, sumEx);
}

double field::getEy(const point& coord) const
{
    auto sumEy = [&coord](double Ey, const charge& charge) -> double {
        const auto& charge_coord = charge.get_coord();
        double dx = coord.x - charge_coord.x;
        double dy = coord.y - charge_coord.y;
        return Ey +
               charge.get_value() * dy / pow(pow(dx, 2.0) + pow(dy, 2.0), 1.5);
    };

    return std::accumulate(_positiveCharges.cbegin(), _positiveCharges.cend(),
                           0.0, sumEy) +
           std::accumulate(_negativeCharges.cbegin(), _negativeCharges.cend(),
                           0.0, sumEy);
}

double field::getE(const point& coord) const
{
    return pow(pow(getEx(coord), 2.0) + pow(getEy(coord), 2.0), 0.5);
}

double field::getCos(const point& coord) const
{
    return getEx(coord) / getE(coord);
}

double field::getSin(const point& coord) const
{
    return getEy(coord) / getE(coord);
}

std::optional<point> field::isComeToNegative(const point& coord) const
{
    return isNear(coord, charge::type::negative);
}

std::optional<point> field::isComeToPositive(const point& coord) const
{
    return isNear(coord, charge::type::positive);
}

const field::Data& field::getPositiveCharges() const
{
    return _positiveCharges;
}

const field::Data& field::getNegativeCharges() const
{
    return _negativeCharges;
}

std::optional<point> field::isNear(const point& coord, charge::type type) const
{
    const auto pred = [&coord](const charge& charge) {
        const double delta = 100;
        const auto& charge_coord = charge.get_coord();
        return pow(charge_coord.x - coord.x, 2.0) +
                   pow(charge_coord.y - coord.y, 2.0) <
               delta;
    };
    switch (type) {
    case charge::type::negative: {
        auto it = std::find_if(_negativeCharges.cbegin(),
                               _negativeCharges.cend(), pred);
        if (it != _negativeCharges.cend()) {
            return it->get_coord();
        }
        break;
    }
    case charge::type::positive: {
        auto it = std::find_if(_positiveCharges.cbegin(),
                               _positiveCharges.cend(), pred);
        if (it != _positiveCharges.cend()) {
            return it->get_coord();
        }
        break;
    }
    }
    return {};
}

} // namespace maxwell
