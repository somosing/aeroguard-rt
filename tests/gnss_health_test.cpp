#include "aeroguard/gnss_health.hpp"

#include <chrono>
#include <iostream>

int main()
{
    using namespace std::chrono_literals;

    int failures = 0;

    const auto check = [&failures](bool condition, const char* name)
    {
        if (!condition)
        {
            std::cerr << "FAILED: " << name << '\n';
            ++failures;
        }
    };

    const std::chrono::steady_clock::time_point t0{};

    {
        aeroguard::GnssHealthMonitor monitor{{1500ms}};

        check(monitor.evaluate(t0) == aeroguard::GnssState::NoData, "no sample gives NoData");
    }

    {
        aeroguard::GnssHealthMonitor monitor{{1500ms}};
        monitor.update({aeroguard::GnssFix::Fix3D, 12, t0});

        check(monitor.evaluate(t0 + 100ms) == aeroguard::GnssState::Nominal,
              "fresh 3D fix gives Nominal");
    }

    {
        aeroguard::GnssHealthMonitor monitor{{1500ms}};
        monitor.update({aeroguard::GnssFix::Fix2D, 6, t0});

        check(monitor.evaluate(t0 + 100ms) == aeroguard::GnssState::Degraded,
              "fresh 2D fix gives Degraded");
    }

    {
        aeroguard::GnssHealthMonitor monitor{{1500ms}};
        monitor.update({aeroguard::GnssFix::NoFix, 0, t0});

        check(monitor.evaluate(t0 + 100ms) == aeroguard::GnssState::Lost, "NoFix gives Lost");
    }

    {
        aeroguard::GnssHealthMonitor monitor{{1500ms}};
        monitor.update({aeroguard::GnssFix::Fix3D, 12, t0});

        check(monitor.evaluate(t0 + 1500ms) == aeroguard::GnssState::Stale,
              "old sample gives Stale");
    }

    if (failures != 0)
    {
        std::cerr << failures << " GNSS health test(s) failed\n";
        return 1;
    }

    std::cout << "All GNSS health tests passed\n";
    return 0;
}
