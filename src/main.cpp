#include "aeroguard/gnss_health.hpp"
#include "aeroguard/gnss_transition_logger.hpp"
#include "aeroguard/version.hpp"

#include <chrono>
#include <iostream>
#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/telemetry/telemetry.h>
#include <mavsdk/system.h>
#include <thread>

namespace
{

aeroguard::GnssFix to_gnss_fix(mavsdk::Telemetry::FixType fix)
{
    switch (fix)
    {
    case mavsdk::Telemetry::FixType::NoGps:
        return aeroguard::GnssFix::NoGps;
    case mavsdk::Telemetry::FixType::NoFix:
        return aeroguard::GnssFix::NoFix;
    case mavsdk::Telemetry::FixType::Fix2D:
        return aeroguard::GnssFix::Fix2D;
    case mavsdk::Telemetry::FixType::Fix3D:
        return aeroguard::GnssFix::Fix3D;
    case mavsdk::Telemetry::FixType::FixDgps:
        return aeroguard::GnssFix::FixDgps;
    case mavsdk::Telemetry::FixType::RtkFloat:
        return aeroguard::GnssFix::RtkFloat;
    case mavsdk::Telemetry::FixType::RtkFixed:
        return aeroguard::GnssFix::RtkFixed;
    }

    return aeroguard::GnssFix::NoFix;
}

} // namespace

int main()
{
    using namespace std::chrono_literals;

    std::cout << "AeroGuard-RT " << aeroguard::version() << '\n';

    mavsdk::Mavsdk mav{mavsdk::Mavsdk::Configuration{mavsdk::ComponentType::CompanionComputer}};

    const auto connection_result = mav.add_any_connection("udpin://0.0.0.0:14540");

    if (connection_result != mavsdk::ConnectionResult::Success)
    {
        std::cerr << "Connection failed\n";
        return 1;
    }

    auto maybe_system = mav.first_autopilot(3.0);

    if (!maybe_system)
    {
        std::cerr << "Autopilot not discovered\n";
        return 1;
    }

    auto system = *maybe_system;

    std::cout << "Autopilot discovered (system ID: " << static_cast<int>(system->get_system_id())
              << ")\n";

    mavsdk::Telemetry telemetry{system};

    aeroguard::GnssHealthMonitor gnss_monitor{aeroguard::GnssMonitorConfig{1500ms}};
    aeroguard::GnssTransitionLogger gnss_logger{std::cout};

    const auto gps_handle = telemetry.subscribe_gps_info(
        [&gnss_monitor](mavsdk::Telemetry::GpsInfo gps_info)
        {
            gnss_monitor.update({
                to_gnss_fix(gps_info.fix_type),
                gps_info.num_satellites,
                std::chrono::steady_clock::now(),
            });
        });

    (void)gps_handle;

    bool previous_connected = system->is_connected();

    std::cout << "[connection] " << (previous_connected ? "CONNECTED" : "DISCONNECTED") << '\n';

    while (true)
    {
        const bool connected = system->is_connected();

        if (connected != previous_connected)
        {
            std::cout << "[connection] " << (connected ? "CONNECTED" : "DISCONNECTED") << '\n';

            previous_connected = connected;
        }

        gnss_logger.observe(gnss_monitor.evaluate(std::chrono::steady_clock::now()));

        std::this_thread::sleep_for(100ms);
    }
}
