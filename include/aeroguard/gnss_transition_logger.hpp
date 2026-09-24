#pragma once

#include "aeroguard/gnss_health.hpp"

#include <iosfwd>

namespace aeroguard
{

class GnssTransitionLogger
{
  public:
    explicit GnssTransitionLogger(std::ostream& output) noexcept;

    void observe(const GnssEvaluation& evaluation);

  private:
    std::ostream& output_;
    GnssState previous_state_{GnssState::NoData};
};

} // namespace aeroguard
