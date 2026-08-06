#pragma once

#include <string_view>

namespace nexus::core
{
    [[nodiscard]] constexpr std::wstring_view app_name() noexcept
    {
        return L"Nexus";
    }

    [[nodiscard]] constexpr std::wstring_view app_tagline() noexcept
    {
        return L"A lean native Windows workspace.";
    }

    [[nodiscard]] std::string_view version() noexcept;
}
