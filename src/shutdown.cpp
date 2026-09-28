#include "aeroguard/shutdown.hpp"

#include <csignal>

namespace
{

volatile std::sig_atomic_t g_shutdown_signal{0};

void handle_shutdown_signal(int signal) noexcept
{
    g_shutdown_signal = signal;
}

} // namespace

namespace aeroguard
{

bool install_shutdown_signal_handlers() noexcept
{
    const auto sigint_result = std::signal(SIGINT, handle_shutdown_signal);
    const auto sigterm_result = std::signal(SIGTERM, handle_shutdown_signal);

    return sigint_result != SIG_ERR && sigterm_result != SIG_ERR;
}

bool shutdown_requested() noexcept
{
    return g_shutdown_signal != 0;
}

int shutdown_signal() noexcept
{
    return static_cast<int>(g_shutdown_signal);
}

std::string_view signal_name(int signal) noexcept
{
    switch (signal)
    {
    case SIGINT:
        return "SIGINT";
    case SIGTERM:
        return "SIGTERM";
    default:
        return "UNKNOWN";
    }
}

} // namespace aeroguard
