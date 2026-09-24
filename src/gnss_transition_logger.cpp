#include "aeroguard/gnss_transition_logger.hpp"

#include <ostream>

namespace aeroguard
{

GnssTransitionLogger::GnssTransitionLogger(std::ostream& output) noexcept : output_{output} {}

void GnssTransitionLogger::observe(const GnssEvaluation& evaluation)
{
    if (evaluation.state == previous_state_)
    {
        return;
    }

    output_ << "[gnss] " << to_string(previous_state_) << " -> " << to_string(evaluation.state);

    if (evaluation.state == GnssState::Stale && evaluation.sample_age)
    {
        const auto sample_age_ms = evaluation.sample_age->count();
        const auto threshold_ms = evaluation.stale_threshold.count();

        output_ << " sample_age_ms=" << sample_age_ms << " threshold_ms=" << threshold_ms
                << " threshold_overshoot_ms=" << (sample_age_ms - threshold_ms);
    }

    output_ << '\n';
    previous_state_ = evaluation.state;
}

} // namespace aeroguard
