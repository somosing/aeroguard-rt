#pragma once

#include <chrono>
#include <cstdint>
#include <mutex>
#include <optional>
#include <string_view>

namespace aeroguard
{

enum class GnssFix
{
    NoGps,
    NoFix,
    Fix2D,
    Fix3D,
    FixDgps,
    RtkFloat,
    RtkFixed
};

enum class GnssState
{
    NoData,
    Nominal,
    Degraded,
    Lost,
    Stale
};

[[nodiscard]] std::string_view to_string(GnssState state) noexcept;

struct GnssSample
{
    GnssFix fix{GnssFix::NoGps};
    std::int32_t satellites{0};
    std::chrono::steady_clock::time_point received_at{};
};

struct GnssMonitorConfig
{
    std::chrono::milliseconds stale_after{1500};
};

struct GnssEvaluation
{
    GnssState state;
    std::optional<std::chrono::milliseconds> sample_age;
    std::chrono::milliseconds stale_threshold;
};

class GnssHealthMonitor
{
  public:
    explicit GnssHealthMonitor(GnssMonitorConfig config);

    void update(const GnssSample& sample);

    [[nodiscard]] GnssEvaluation evaluate(std::chrono::steady_clock::time_point now) const;

  private:
    GnssMonitorConfig config_;
    std::optional<GnssSample> latest_sample_;
    mutable std::mutex mutex_;
};

} // namespace aeroguard
