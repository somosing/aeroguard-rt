#pragma once

#include "aeroguard/gnss_health.hpp"

#include <string_view>

namespace aeroguard
{

enum class WatchdogDecision
{
    Normal,
    Caution,
    InterventionRequired,
};

[[nodiscard]] WatchdogDecision evaluate_watchdog(bool connected, GnssState gnss_state) noexcept;
[[nodiscard]] std::string_view to_string(WatchdogDecision decision) noexcept;

} // namespace aeroguard
