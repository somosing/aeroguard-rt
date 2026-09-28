#pragma once

#include <string_view>

namespace aeroguard
{

[[nodiscard]] bool install_shutdown_signal_handlers() noexcept;
[[nodiscard]] bool shutdown_requested() noexcept;
[[nodiscard]] int shutdown_signal() noexcept;
[[nodiscard]] std::string_view signal_name(int signal) noexcept;

} // namespace aeroguard
