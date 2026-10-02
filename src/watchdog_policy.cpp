#include "aeroguard/watchdog_policy.hpp"

namespace aeroguard
{

WatchdogDecision evaluate_watchdog(bool connected, GnssState gnss_state) noexcept
{
    if (!connected)
    {
        return WatchdogDecision::InterventionRequired;
    }

    switch (gnss_state)
    {
    case GnssState::Nominal:
        return WatchdogDecision::Normal;

    case GnssState::NoData:
    case GnssState::Degraded:
        return WatchdogDecision::Caution;

    case GnssState::Lost:
    case GnssState::Stale:
        return WatchdogDecision::InterventionRequired;
    }

    return WatchdogDecision::InterventionRequired;
}

std::string_view to_string(WatchdogDecision decision) noexcept
{
    switch (decision)
    {
    case WatchdogDecision::Normal:
        return "NORMAL";
    case WatchdogDecision::Caution:
        return "CAUTION";
    case WatchdogDecision::InterventionRequired:
        return "INTERVENTION_REQUIRED";
    }

    return "UNKNOWN";
}

} // namespace aeroguard
