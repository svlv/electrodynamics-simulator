#include "physics/field.hpp"
#include "utils.hpp"

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

double field::get_angle(const point& coord) const
{
    return maxwell::get_angle(getCos(coord), getSin(coord));
}

double field::getEx(const point& coord) const
{
    const auto sumEx = [&coord](double Ex, const charge& charge) -> double {
        const auto delta = coord - charge.get_coord();
        return Ex + charge.get_value() * delta.x / pow(delta.module(), 3.0);
    };

    return std::accumulate(_positiveCharges.cbegin(), _positiveCharges.cend(),
                           0.0, sumEx) +
           std::accumulate(_negativeCharges.cbegin(), _negativeCharges.cend(),
                           0.0, sumEx);
}

double field::getEy(const point& coord) const
{
    auto sumEy = [&coord](double Ey, const charge& charge) -> double {
        const auto delta = coord - charge.get_coord();
        return Ey + charge.get_value() * delta.y / pow(delta.module(), 3.0);
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
        const double min_delta = 10.0;
        const auto delta = charge.get_coord() - coord;
        return delta.module() < min_delta;
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
