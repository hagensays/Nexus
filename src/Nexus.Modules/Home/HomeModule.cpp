#include "Nexus.Modules/Home/HomeModule.hpp"
#include "Nexus.Core/AppMetadata.hpp"

namespace nexus::modules::home
{
    std::wstring_view title() noexcept
    {
        return nexus::core::app_name();
    }

    std::wstring_view description() noexcept
    {
        return nexus::core::app_tagline();
    }
}
