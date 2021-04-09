#include "physics/field.hpp"
#include "utils.hpp"

#include <algorithm>
#include <cmath>
#include <numeric>

namespace maxwell
{

field::field(const charges& chrgs) : _charges(chrgs) {}

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

    const auto& pos = _charges.get().get_positive_charges();
    const auto& neg = _charges.get().get_negative_charges();

    return std::accumulate(pos.cbegin(), pos.cend(), 0.0, sumEx) +
           std::accumulate(neg.cbegin(), neg.cend(), 0.0, sumEx);
}

double field::getEy(const point& coord) const
{
    auto sumEy = [&coord](double Ey, const charge& charge) -> double {
        const auto delta = coord - charge.get_coord();
        return Ey + charge.get_value() * delta.y / pow(delta.module(), 3.0);
    };

    const auto& pos = _charges.get().get_positive_charges();
    const auto& neg = _charges.get().get_negative_charges();

    return std::accumulate(pos.cbegin(), pos.cend(),
                           0.0, sumEy) +
           std::accumulate(neg.cbegin(), neg.cend(),
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

} // namespace maxwell
