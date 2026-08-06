#pragma once

#include <string_view>

namespace nexus::modules::home
{
    [[nodiscard]] std::wstring_view title() noexcept;
    [[nodiscard]] std::wstring_view description() noexcept;
}
