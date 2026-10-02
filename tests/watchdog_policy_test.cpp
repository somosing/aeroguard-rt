#include "aeroguard/watchdog_policy.hpp"

#include <iostream>

int main()
{
    int failures = 0;

    const auto check = [&failures](bool condition, const char* name)
    {
        if (!condition)
        {
            std::cerr << "FAILED: " << name << '\n';
            ++failures;
        }
    };

    using aeroguard::GnssState;
    using aeroguard::WatchdogDecision;

    check(aeroguard::evaluate_watchdog(true, GnssState::Nominal) == WatchdogDecision::Normal,
          "connected nominal GNSS is normal");

    check(aeroguard::evaluate_watchdog(true, GnssState::Degraded) == WatchdogDecision::Caution,
          "degraded GNSS requires caution");

    check(aeroguard::evaluate_watchdog(true, GnssState::NoData) == WatchdogDecision::Caution,
          "no GNSS data requires caution");

    check(aeroguard::evaluate_watchdog(true, GnssState::Lost) ==
              WatchdogDecision::InterventionRequired,
          "lost GNSS requires intervention");

    check(aeroguard::evaluate_watchdog(true, GnssState::Stale) ==
              WatchdogDecision::InterventionRequired,
          "stale GNSS requires intervention");

    check(aeroguard::evaluate_watchdog(false, GnssState::Nominal) ==
              WatchdogDecision::InterventionRequired,
          "disconnection overrides nominal GNSS");

    if (failures != 0)
    {
        return 1;
    }

    std::cout << "All watchdog policy tests passed\n";
    return 0;
}
