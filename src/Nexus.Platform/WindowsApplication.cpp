#include "Nexus.Platform/WindowsApplication.hpp"
#include "Nexus.UI/MainWindow.hpp"

namespace nexus::platform
{
    int WindowsApplication::run(HINSTANCE instance, int show_command) const
    {
        SetProcessDPIAware();
        nexus::ui::MainWindow main_window;
        return main_window.run(instance, show_command);
    }
}
