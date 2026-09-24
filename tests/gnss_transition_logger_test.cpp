#include "aeroguard/gnss_transition_logger.hpp"

#include <chrono>
#include <iostream>
#include <sstream>
#include <string>

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

    std::ostringstream output;
    aeroguard::GnssTransitionLogger logger{output};

    logger.observe({aeroguard::GnssState::NoData, std::nullopt, 1500ms});
    check(output.str().empty(), "unchanged NoData produces no transition log");

    logger.observe({aeroguard::GnssState::Nominal, 25ms, 1500ms});
    check(output.str() == "[gnss] NO_DATA -> NOMINAL\n", "logs initial GNSS transition");

    logger.observe({aeroguard::GnssState::Nominal, 50ms, 1500ms});
    check(output.str() == "[gnss] NO_DATA -> NOMINAL\n",
          "unchanged state produces no duplicate log");

    logger.observe({aeroguard::GnssState::Stale, 1504ms, 1500ms});
    check(output.str() == "[gnss] NO_DATA -> NOMINAL\n"
                          "[gnss] NOMINAL -> STALE sample_age_ms=1504 threshold_ms=1500 "
                          "threshold_overshoot_ms=4\n",
          "stale transition logs freshness timing");

    logger.observe({aeroguard::GnssState::Nominal, 10ms, 1500ms});
    check(output.str() == "[gnss] NO_DATA -> NOMINAL\n"
                          "[gnss] NOMINAL -> STALE sample_age_ms=1504 threshold_ms=1500 "
                          "threshold_overshoot_ms=4\n"
                          "[gnss] STALE -> NOMINAL\n",
          "recovery transition stays concise");

    if (failures != 0)
    {
        std::cerr << failures << " GNSS transition logger test(s) failed\n";
        return 1;
    }

    std::cout << "All GNSS transition logger tests passed\n";
    return 0;
}
