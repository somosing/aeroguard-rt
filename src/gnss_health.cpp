#include "aeroguard/gnss_health.hpp"

#include <mutex>

namespace aeroguard
{

std::string_view to_string(GnssState state) noexcept
{
    switch (state)
    {
    case GnssState::NoData:
        return "NO_DATA";
    case GnssState::Nominal:
        return "NOMINAL";
    case GnssState::Degraded:
        return "DEGRADED";
    case GnssState::Lost:
        return "LOST";
    case GnssState::Stale:
        return "STALE";
    }

    return "UNKNOWN";
}

GnssHealthMonitor::GnssHealthMonitor(GnssMonitorConfig config) : config_{config} {}

void GnssHealthMonitor::update(const GnssSample& sample)
{
    const std::lock_guard lock{mutex_};
    latest_sample_ = sample;
}

GnssState GnssHealthMonitor::evaluate(std::chrono::steady_clock::time_point now) const
{
    const std::lock_guard lock{mutex_};

    if (!latest_sample_)
    {
        return GnssState::NoData;
    }

    const auto age = now - latest_sample_->received_at;

    if (age >= config_.stale_after)
    {
        return GnssState::Stale;
    }

    switch (latest_sample_->fix)
    {
    case GnssFix::NoGps:
    case GnssFix::NoFix:
        return GnssState::Lost;

    case GnssFix::Fix2D:
        return GnssState::Degraded;

    case GnssFix::Fix3D:
    case GnssFix::FixDgps:
    case GnssFix::RtkFloat:
    case GnssFix::RtkFixed:
        return GnssState::Nominal;
    }

    return GnssState::Lost;
}

} // namespace aeroguard
