#include "aeroguard/shutdown.hpp"

#include <csignal>
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

    check(aeroguard::install_shutdown_signal_handlers(), "installs shutdown signal handlers");
    check(!aeroguard::shutdown_requested(), "shutdown not requested initially");

    std::raise(SIGINT);

    check(aeroguard::shutdown_requested(), "SIGINT requests shutdown");
    check(aeroguard::shutdown_signal() == SIGINT, "records SIGINT");
    check(aeroguard::signal_name(SIGINT) == "SIGINT", "formats SIGINT");
    check(aeroguard::signal_name(SIGTERM) == "SIGTERM", "formats SIGTERM");
    check(aeroguard::signal_name(0) == "UNKNOWN", "formats unknown signal");

    if (failures != 0)
    {
        std::cerr << failures << " shutdown test(s) failed\n";
        return 1;
    }

    std::cout << "All shutdown tests passed\n";
    return 0;
}
