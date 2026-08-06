#include "Nexus.Modules/Settings/SettingsModule.hpp"

namespace nexus::modules::settings
{
    std::wstring_view title() noexcept
    {
        return L"Settings";
    }

    std::wstring_view description() noexcept
    {
        return L"Nexus settings will live here.";
    }
}
