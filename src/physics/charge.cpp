#include "charge.hpp"

#include <algorithm>
#include <cmath>
#include <numeric>

namespace maxwell
{

void Charges::clear()
{
    _positiveCharges.clear();
    _negativeCharges.clear();
}

bool Charges::empty()
{
    return _positiveCharges.empty() && _negativeCharges.empty();
}

double Charges::getEx(const point& coordinates) const
{
    const auto sumEx = [&coordinates](double Ex,
                                      const Charge& charge) -> double {
        double dx = coordinates.x - charge.coordinates.x;
        double dy = coordinates.y - charge.coordinates.y;
        return Ex + charge.value * dx / pow(pow(dx, 2.0) + pow(dy, 2.0), 1.5);
    };

    return std::accumulate(_positiveCharges.cbegin(), _positiveCharges.cend(),
                           0.0, sumEx) +
           std::accumulate(_negativeCharges.cbegin(), _negativeCharges.cend(),
                           0.0, sumEx);
}

double Charges::getEy(const point& coordinates) const
{
    auto sumEy = [&coordinates](double Ey, const Charge& charge) -> double {
        double dx = coordinates.x - charge.coordinates.x;
        double dy = coordinates.y - charge.coordinates.y;
        return Ey + charge.value * dy / pow(pow(dx, 2.0) + pow(dy, 2.0), 1.5);
    };

    return std::accumulate(_positiveCharges.cbegin(), _positiveCharges.cend(),
                           0.0, sumEy) +
           std::accumulate(_negativeCharges.cbegin(), _negativeCharges.cend(),
                           0.0, sumEy);
}

double Charges::getE(const point& coordinates) const
{
    return pow(pow(getEx(coordinates), 2.0) + pow(getEy(coordinates), 2.0),
               0.5);
}

double Charges::getCos(const point& coordinates) const
{
    return getEx(coordinates) / getE(coordinates);
}

double Charges::getSin(const point& coordinates) const
{
    return getEy(coordinates) / getE(coordinates);
}

std::optional<point> Charges::isComeToNegative(const point& coordinates) const
{
    return isNear(coordinates, ChargeType::Negative);
}

std::optional<point> Charges::isComeToPositive(const point& coordinates) const
{
    return isNear(coordinates, ChargeType::Positive);
}

const Charges::Data& Charges::getPositiveCharges() const
{
    return _positiveCharges;
}

const Charges::Data& Charges::getNegativeCharges() const
{
    return _negativeCharges;
}

std::optional<point> Charges::isNear(const point& coordinates,
                                     ChargeType type) const
{
    const auto pred = [&coordinates](const Charge& charge) {
        const double delta = 100;
        return pow(charge.coordinates.x - coordinates.x, 2.0) +
                   pow(charge.coordinates.y - coordinates.y, 2.0) <
               delta;
    };
    switch (type) {
    case ChargeType::Negative: {
        auto it = std::find_if(_negativeCharges.cbegin(),
                               _negativeCharges.cend(), pred);
        if (it != _negativeCharges.cend()) {
            return it->coordinates;
        }
        break;
    }
    case ChargeType::Positive: {
        auto it = std::find_if(_positiveCharges.cbegin(),
                               _positiveCharges.cend(), pred);
        if (it != _positiveCharges.cend()) {
            return it->coordinates;
        }
        break;
    }
    }
    return {};
}

} // namespace maxwell
