#include "aeroguard/version.hpp"
#include <mavsdk/mavsdk.h>
#include <mavsdk/system.h>

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
    return 0;
}
