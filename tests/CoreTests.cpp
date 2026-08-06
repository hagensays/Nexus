#include "Nexus.Core/AppMetadata.hpp"
#include "Nexus.Modules/Home/HomeModule.hpp"
#include "Nexus.Modules/Settings/SettingsModule.hpp"

#include <iostream>

namespace
{
    int failures = 0;

    void expect(bool condition, const char* message)
    {
        if (!condition)
        {
            std::cerr << "FAILED: " << message << '\n';
            ++failures;
        }
    }
}

int main()
{
    expect(nexus::core::app_name() == L"Nexus", "app name");
    expect(!nexus::core::version().empty(), "version is present");
    expect(nexus::modules::home::title() == nexus::core::app_name(), "Home uses app name");
    expect(!nexus::modules::home::description().empty(), "Home description is present");
    expect(nexus::modules::settings::title() == L"Settings", "Settings title");

    if (failures == 0)
    {
        std::cout << "All Nexus Core tests passed.\n";
    }

    return failures == 0 ? 0 : 1;
}
