#include "aeroguard/gnss_health.hpp"

#include <chrono>
#include <mutex>

namespace
{

aeroguard::GnssState state_from_fix(aeroguard::GnssFix fix) noexcept
{
    switch (fix)
    {
    case aeroguard::GnssFix::NoGps:
    case aeroguard::GnssFix::NoFix:
        return aeroguard::GnssState::Lost;

    case aeroguard::GnssFix::Fix2D:
        return aeroguard::GnssState::Degraded;

    case aeroguard::GnssFix::Fix3D:
    case aeroguard::GnssFix::FixDgps:
    case aeroguard::GnssFix::RtkFloat:
    case aeroguard::GnssFix::RtkFixed:
        return aeroguard::GnssState::Nominal;
    }

    return aeroguard::GnssState::Lost;
}

} // namespace

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

GnssEvaluation GnssHealthMonitor::evaluate(std::chrono::steady_clock::time_point now) const
{
    const std::lock_guard lock{mutex_};

    if (!latest_sample_)
    {
        return {GnssState::NoData, std::nullopt, config_.stale_after};
    }

    const auto age = now - latest_sample_->received_at;
    const auto age_ms = std::chrono::duration_cast<std::chrono::milliseconds>(age);

    if (age >= config_.stale_after)
    {
        return {GnssState::Stale, age_ms, config_.stale_after};
    }

    return {state_from_fix(latest_sample_->fix), age_ms, config_.stale_after};
}

} // namespace aeroguard
