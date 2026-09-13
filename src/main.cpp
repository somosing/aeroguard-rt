#include "aeroguard/version.hpp"
#include <chrono>
#include <mavsdk/mavsdk.h>
#include <mavsdk/system.h>
#include <thread>

#include <iostream>

int main()
{
    std::cout << "AeroGuard-RT " << aeroguard::version() << '\n';
    mavsdk::Mavsdk mav{mavsdk::Mavsdk::Configuration{mavsdk::ComponentType::CompanionComputer}};
    auto connection_result = mav.add_any_connection("udpin://0.0.0.0:14540");
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

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    return 0;
}
