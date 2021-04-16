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
    return maxwell::get_angle(get_cos(coord), get_sin(coord));
}

double field::get_Ex(const point& coord) const
{
    const auto sumEx = [&coord](double Ex, const auto& charge) -> double {
        const auto delta = coord - charge->get_coord();
        return Ex + charge->get_value() * delta.x / pow(delta.module(), 3.0);
    };

    const auto& pos = _charges.get().get_positive_charges();
    const auto& neg = _charges.get().get_negative_charges();

    return std::accumulate(pos.cbegin(), pos.cend(), 0.0, sumEx) +
           std::accumulate(neg.cbegin(), neg.cend(), 0.0, sumEx);
}

double field::get_Ey(const point& coord) const
{
    auto sumEy = [&coord](double Ey, const auto& charge) -> double {
        const auto delta = coord - charge->get_coord();
        return Ey + charge->get_value() * delta.y / pow(delta.module(), 3.0);
    };

    const auto& pos = _charges.get().get_positive_charges();
    const auto& neg = _charges.get().get_negative_charges();

    return std::accumulate(pos.cbegin(), pos.cend(), 0.0, sumEy) +
           std::accumulate(neg.cbegin(), neg.cend(), 0.0, sumEy);
}

double field::get_E(const point& coord) const
{
    return pow(pow(get_Ex(coord), 2.0) + pow(get_Ey(coord), 2.0), 0.5);
}

double field::get_cos(const point& coord) const
{
    return get_Ex(coord) / get_E(coord);
}

double field::get_sin(const point& coord) const
{
    return get_Ey(coord) / get_E(coord);
}

} // namespace maxwell
